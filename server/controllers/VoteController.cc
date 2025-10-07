#include "VoteController.h"
#include "../models/TestVoteData.h"
#include "../models/TestVoteItem.h"
#include "../models/TestVoteParticipateInfo.h"
#include "../models/TestVoteUser.h"
#include "VoteReqJson.h"
#include "util.h"
#include <mutex>




// Add definition of your processing function here
using namespace tang::common;
using namespace tang::server::utils;
using namespace tang::server;

using VoteData            = drogon_model::vote::TestVoteData;
using VoteUser            = drogon_model::vote::TestVoteUser;
using VoteItem            = drogon_model::vote::TestVoteItem;
using VoteParticipateInfo = drogon_model::vote::TestVoteParticipateInfo;


struct VoteCacheConfig {
    static constexpr size_t max_vote_cache_num = 520;
};

// maybe used in the future!
struct VoteJsonValidator {
    static constexpr const char* vote_topic_key                      = "vote_topic";
    static constexpr const char* vote_items_key                      = "vote_items";
    static constexpr const char* vote_creator_id_key                 = "vote_creator_id";
    static constexpr const char* vote_creator_key                    = "vote_creator";
    static constexpr const char* vote_create_time_key                = "vote_create_time";
    static constexpr const char* voters_key                          = "voters";
    static constexpr const char* voter_names_key                     = "voter_names";
    static constexpr const char* vote_choice_type_key                = "vote_choice_type";
    static constexpr size_t      vote_key_num                        = 7;
    static constexpr std::array<const char*, vote_key_num> json_keys = {vote_topic_key,
                                                                        vote_items_key,
                                                                        vote_creator_id_key,
                                                                        vote_creator_key,
                                                                        vote_create_time_key,
                                                                        voters_key,
                                                                        vote_choice_type_key};

    static StatusCode validate_vote_data(const Json::Value& json_data) {
        // validate whether the key exist!
        for (size_t i = 0; i < json_keys.size(); ++i) {
            if (!json_data.isMember(json_keys[i])) {
                LOG_ERROR << "Missing json key:" << json_keys[i];
                return StatusCode::kJsonKeyError;
            }
        }
        // validate the type!
        if (!json_data[vote_topic_key].isString()) {
            return StatusCode::kJsonTypeError;
        }

        if (!json_data[vote_items_key].isArray()) {
            return StatusCode::kJsonTypeError;
        }
        for (auto& item : json_data[vote_items_key]) {
            if (!item.isString()) {
                return StatusCode::kJsonTypeError;
            }
        }

        if (!json_data[vote_creator_key].isString()) {
            return StatusCode::kJsonTypeError;
        }

        if (!json_data[vote_create_time_key].isInt64()) {
            return StatusCode::kJsonTypeError;
        }

        if (!json_data[voters_key].isArray()) {
            return StatusCode::kJsonTypeError;
        }

        for (auto& item : json_data[voters_key]) {
            if (!item.isUInt()) {
                return StatusCode::kJsonTypeError;
            }
        }

        if (!json_data[vote_choice_type_key].isInt()) {
            return StatusCode::kJsonTypeError;
        }

        return StatusCode::kSuccess;
    }
};


bool parse_create_vote_params(const Json::Value& json_data, CreateVoteReqParams& params) {
    params.vote_topic = json_data[VoteJsonValidator::vote_topic_key].asString();
    auto  n_item      = json_data[VoteJsonValidator::vote_items_key].size();
    auto& vote_items  = params.vote_items;
    vote_items.reserve(n_item);
    for (auto& item : json_data[VoteJsonValidator::vote_items_key]) {
        vote_items.push_back(item.asString());
    }
    params.vote_creator    = json_data[VoteJsonValidator::vote_creator_key].asString();
    params.vote_creator_id = json_data[VoteJsonValidator::vote_creator_id_key].asInt();

    params.vote_create_time = json_data[VoteJsonValidator::vote_create_time_key].asInt64();
    auto& voter_ids         = params.voter_ids;
    auto  n_voters          = json_data[VoteJsonValidator::voters_key].size();
    voter_ids.reserve(n_voters);
    for (auto& item : json_data[VoteJsonValidator::voters_key]) {
        uint32_t voter_id = item.asUInt();
        voter_ids.push_back(voter_id);
    }

    auto& voter_names      = params.voter_names;
    auto& json_voter_names = json_data[VoteJsonValidator::voter_names_key];
    auto  n_voter_names    = json_voter_names.size();
    if (n_voter_names != n_voters) {
        return false;
    }
    voter_names.reserve(n_voter_names);
    for (auto& item : json_voter_names) {
        voter_names.push_back(item.asString());
    }


    auto choice_type_value = json_data[VoteJsonValidator::vote_choice_type_key].asInt();
    if (choice_type_value >= vote_choice_type_count) {
        LOG_ERROR << "Invalid choice type:" << choice_type_value;
        return false;
    }
    params.choice_type = static_cast<VoteChoiceType>(choice_type_value);
    return true;
}

void VoteController::create_vote(const HttpRequestPtr&                         req,
                                 std::function<void(const HttpResponsePtr&)>&& callback) {
    // parse it from the json!
    auto json_data = req->getJsonObject();
    if (json_data == nullptr) {
        LOG_ERROR << "Fail to get json param,please give right!!!";
        make_response_and_return(StatusCode::kJsonParamIsNull, callback);
    }
    if (auto ret = VoteJsonValidator::validate_vote_data(*json_data); ret != StatusCode::kSuccess) {
        make_response_and_return(ret, callback);
    }
    CreateVoteReqParams params;
    if (!parse_create_vote_params(*json_data, params)) {
        make_response_and_return(StatusCode::kLogicError, callback);
    }
    //******************************** */
    VoteCacheInfo vote_cache_info;
    auto          n_items = params.vote_items.size();
    vote_cache_info.vote_item_ids.reserve(n_items);
    // construct
    auto& voter_infos = vote_cache_info.voter_infos;
    auto& voter_ids   = params.voter_ids;
    for (auto voter_id : voter_ids) {
        VoterInfo voter_info{{}, 0, VoteProcessStatus::kReady};
        voter_infos.emplace(voter_id, voter_info);
    }
    vote_cache_info.vote_creator_id = params.vote_creator_id;
    //*************************************************************** */
    auto db_client_ptr = app().getDbClient(get_db_client_name());
    if (db_client_ptr == nullptr) {
        LOG_ERROR << "database ptr is invalid...";
        make_response_and_return(StatusCode::kInvalidDatabase, callback);
    }
    auto                             trans = db_client_ptr->newTransaction();
    orm::Mapper<VoteData>            vote_data_mapper(trans);
    orm::Mapper<VoteItem>            vote_item_mapper(trans);
    orm::Mapper<VoteParticipateInfo> vote_participate_mapper(trans);

    VoteData insert_vote_data;
    insert_vote_data.setVoteTopic(params.vote_topic);
    insert_vote_data.setVoteCreator(params.vote_creator);
    insert_vote_data.setVoteCreatorId(params.vote_creator_id),
        insert_vote_data.setVoteCreateTime(::trantor::Date(params.vote_create_time));
    insert_vote_data.setVoteType(static_cast<uint8_t>(params.choice_type));
    try {
        vote_data_mapper.insert(insert_vote_data);
        auto vote_id           = insert_vote_data.getValueOfVoteId();
        auto vote_creator_name = insert_vote_data.getValueOfVoteCreator();
        LOG_INFO << "Successfully add vote, vote_id:" << vote_id
                 << " vote_topic:" << params.vote_topic;
        auto& vote_item_ids = vote_cache_info.vote_item_ids;
        vote_item_ids.resize(n_items, 0);
        auto& vote_items = params.vote_items;
        // for the vote items...
        for (size_t i = 0; i < n_items; ++i) {
            VoteItem insert_vote_item;
            insert_vote_item.setVoteItem(vote_items[i]);
            insert_vote_item.setVoteId(vote_id);
            insert_vote_item.setVoteItemCount(0);
            vote_item_mapper.insert(insert_vote_item);
            vote_item_ids[i] = insert_vote_item.getValueOfId();
        }
        // for the voters...
        size_t n_voter = params.voter_ids.size();
        for (size_t i = 0; i < n_voter; ++i) {
            VoteParticipateInfo insert_participate;
            insert_participate.setVoteId(vote_id);
            insert_participate.setVoterId(params.voter_ids[i]);
            insert_participate.setVoterName(params.voter_names[i]);
            vote_participate_mapper.insert(insert_participate);
            auto  participate_id      = insert_participate.getValueOfId();
            auto& voter_info          = vote_cache_info.voter_infos.at(params.voter_ids[i]);
            voter_info.participate_id = participate_id;
        }

        if (auto ret = this->add_vote_data_to_cache(std::move(vote_cache_info), vote_id);
            ret != StatusCode::kSuccess) {
            make_response_and_return(ret, callback);
            // should reset status -> invalid???
        }
        // here should return a vote_id?
        auto ret       = make_json_from_status_code(StatusCode::kSuccess);
        ret["vote_id"] = vote_id;
        auto resp      = HttpResponse::newHttpJsonResponse(ret);
        callback(resp);

    } catch (const orm::DrogonDbException& ex) {
        LOG_ERROR << "Fail to insert data with error:" << ex.base().what();
        trans->rollback();
        make_response_and_return(StatusCode::kSqlRuntimeError, callback);
    }
}


void VoteController::remove_vote(const HttpRequestPtr&                         req,
                                 std::function<void(const HttpResponsePtr&)>&& callback) {
    auto vote_id_str         = req->getParameter("vote_id");
    auto vote_creator_id_str = req->getParameter("vote_creator_id");
    if (vote_id_str.empty() || vote_creator_id_str.empty()) {
        make_response_and_return(StatusCode::kInvalidVoteid, callback);
    }
    // then convert to the int32_t
    uint32_t vote_id;
    uint32_t vote_creator_id;

    // got the error!
    auto result =
        std::from_chars(vote_id_str.data(), vote_id_str.data() + vote_id_str.size(), vote_id);
    if (result.ec != std::errc()) {
        LOG_ERROR << "Fail to convert vote_id:" << vote_id_str << " to int32_t";
        make_response_and_return(StatusCode::kInvalidVoteid, callback);
    }

    result = std::from_chars(vote_creator_id_str.data(),
                             vote_creator_id_str.data() + vote_creator_id_str.size(),
                             vote_creator_id);
    if (result.ec != std::errc()) {
        LOG_ERROR << "Fail to convert vote creator id:" << vote_creator_id_str << " to int32_t";
        make_response_and_return(StatusCode::kInvalidVoteid, callback);
    }

    {
        std::lock_guard<std::mutex> guard(this->vote_mt);
        auto                        it = this->vote_datas.find(vote_id);
        if (it != this->vote_datas.end() && vote_creator_id == it->second.vote_creator_id) {
            this->vote_datas.erase(it);
            LOG_INFO << "Successfully remove vote_id:" << vote_id << " from memory!";
        } else {
            LOG_ERROR << "Fail to erase from the memory,maybe not the creator or already erase?";
        }
    }

    // then mark it as invalid in the database!
    auto db_client_ptr = app().getDbClient(get_db_client_name());
    if (db_client_ptr == nullptr) {
        // wow,the databas is nullptr!
        // here we do not need
        LOG_ERROR << "database ptr is invalid...";
        make_response_and_return(StatusCode::kInvalidDatabase, callback);
    }

    orm::Mapper<VoteData> vote_data_mapper(db_client_ptr);
    auto                  c1 = [callback](size_t n) {
        if (n == 0) {
            LOG_ERROR << "maybe not have the permisson or already mark invalid from the database?";
            make_response_and_return(StatusCode::kFailRemoveVote, callback);
        }
        make_response_only(StatusCode::kSuccess, callback);
    };
    auto c2 = [callback](const orm::DrogonDbException& ex) {
        LOG_ERROR << "sql error:" << ex.base().what();
        make_response_from_status_code(StatusCode::kSqlRuntimeError, callback);
    };

    vote_data_mapper.updateBy(
        {VoteData::Cols::_vote_status},
        c1,
        c2,
        orm::Criteria(VoteData::Cols::_vote_id, orm::CompareOperator::EQ, vote_id) &&
            orm::Criteria(
                VoteData::Cols::_vote_creator_id, orm::CompareOperator::EQ, vote_creator_id) &&

            orm::Criteria(VoteData::Cols::_vote_status,
                          orm::CompareOperator::EQ,
                          static_cast<uint8_t>(VoteStatus::kReady)),
        VoteStatus::kInvalid);

    // change to vote_status is better!
}


SendChoicesProcessResult VoteController::process_voter_choices_impl(
    const SendVoteChoicesParams& params) {
    SendChoicesProcessResult    process_result;
    std::lock_guard<std::mutex> guard(this->vote_mt);
    if (!vote_datas.contains(params.vote_id)) {
        LOG_ERROR << "The vote_id:" << params.vote_id << " is not exist!";
        process_result.ret = StatusCode::KVoteIdNotFound;
        return process_result;
    }
    auto& vote_cache_info = vote_datas[params.vote_id];
    if (vote_cache_info.choice_type == VoteChoiceType::kSingleChoice &&
        params.voter_choices.size() != 1) {
        LOG_ERROR << "The vote data,vote_id:" << params.vote_id
                  << " is single choice type,but get multi choices which is unexpeced!";
        process_result.ret = StatusCode::kVoteChoceTypeMismatch;
        return process_result;
    }
    int n_vote_item = vote_cache_info.vote_item_ids.size();
    for (auto choice : params.voter_choices) {
        if (choice < 0 || choice >= n_vote_item) {
            process_result.ret = StatusCode::kInvalidVoteChoice;
            return process_result;
        }
    }
    if (!vote_cache_info.voter_infos.contains(params.voter_id)) {
        LOG_ERROR << "the voter is not valid!";
        process_result.ret = StatusCode::kVoterIsNotInclude;
        return process_result;
    }
    LOG_INFO << "add to cache...";
    auto& voter_infos = vote_cache_info.voter_infos;
    if (!voter_infos.contains(params.voter_id)) {
        process_result.ret = StatusCode::kVoterIsNotInclude;
        return process_result;
    }
    auto& voter_info   = voter_infos[params.voter_id];
    voter_info.choices = std::move(params.voter_choices);
    vote_cache_info.n_finished++;
    process_result.participate_id = voter_info.participate_id;
    process_result.finished = vote_cache_info.n_finished == vote_cache_info.voter_infos.size();
    return process_result;
}

void VoteController::send_vote_choices(const HttpRequestPtr&                         req,
                                       std::function<void(const HttpResponsePtr&)>&& callback) {
    auto json_data = req->getJsonObject();
    if (json_data == nullptr) {
        LOG_ERROR << "Fail to get json data";
        make_response_and_return(StatusCode::kJsonParamIsNull, callback);
    }
    SendVoteChoicesParams params;
    if (auto ret = parse_send_vote_choices_req(*json_data, params); ret != StatusCode::kSuccess) {
        make_response_and_return(ret, callback);
    }

    auto process_ret = process_voter_choices_impl(params);
    if (process_ret.ret != StatusCode::kSuccess) {
        make_response_and_return(process_ret.ret, callback);
    }
    // then update
    auto db_client_ptr = app().getDbClient(get_db_client_name());
    if (db_client_ptr == nullptr) {
        make_response_and_return(StatusCode::kInvalidDatabase, callback);
    }
    orm::Mapper<VoteParticipateInfo> participate_info_mapper(db_client_ptr);
    try {
        LOG_INFO << "update the participate id to processed ..." << process_ret.participate_id;
        participate_info_mapper.updateBy({VoteParticipateInfo::Cols::_vote_process_status},
                                         orm::Criteria(VoteParticipateInfo::Cols::_id,
                                                       orm::CompareOperator::EQ,
                                                       process_ret.participate_id),
                                         static_cast<int>(VoteProcessStatus::kProcessed));
    } catch (const orm::DrogonDbException& ex) {
        LOG_ERROR << ex.base().what();
        make_response_and_return(StatusCode::kSqlRuntimeError, callback);
    }
    make_response_only(StatusCode::kSuccess, callback);
    if (process_ret.finished) {
        call_when_vote_finished(params.vote_id);
    }
}

StatusCode VoteController::add_vote_data_to_cache(VoteCacheInfo&& cache_info,
                                                  uint32_t        vote_id) noexcept {
    auto guard = std::lock_guard<std::mutex>(this->vote_mt);

    // exceed the maximum of cache!
    if (this->vote_datas.size() >= VoteCacheConfig::max_vote_cache_num) {
        LOG_ERROR << "The vote cache is full,now can not add any vote data to it!";
        return StatusCode::kVoteCacheIsFull;
    }
    if (!this->vote_datas.insert({vote_id, std::move(cache_info)}).second) {
        LOG_ERROR << "Fail to insert vote:" << vote_id;
        return StatusCode::kFailAddVoteToCache;
    }
    return StatusCode::kSuccess;
}

StatusCode VoteController::remove_vote_data_from_cache(uint32_t vote_id) noexcept {
    std::lock_guard<std::mutex> guard(this->vote_mt);
    if (!vote_datas.contains(vote_id)) {
        LOG_ERROR << "Can not remove vote_id:" << vote_id << " which is not exist!";
        return StatusCode::KVoteIdNotFound;
    }
    vote_datas.erase(vote_id);
    return StatusCode::kSuccess;
}

bool VoteController::compute_vote_result(const VoteCacheInfo& cache_info,
                                         VoteResult&          vote_result) const noexcept {
    auto db_client_ptr = app().getDbClient(get_db_client_name());
    if (db_client_ptr == nullptr) {
        LOG_ERROR << "the db client is nullptr!";
        return false;
    }
    auto n_voter = cache_info.voter_infos.size();
    if (n_voter == 0) {
        return false;
    }
    // assert(n_voter > 0);
    std::vector<uint32_t> voter_ids;
    voter_ids.reserve(n_voter);
    // for query
    for (auto& [k, _] : cache_info.voter_infos) {
        voter_ids.push_back(k);
    }

    orm::Mapper<VoteUser> user_mapper(db_client_ptr);
    std::vector<VoteUser> select_result;
    try {
        select_result = user_mapper.findBy(
            orm::Criteria(VoteUser::Cols::_id, orm::CompareOperator::In, voter_ids));
    } catch (const orm::DrogonDbException& ex) {
        LOG_ERROR << ex.base().what();
        return false;
    }
    if (select_result.size() != n_voter) {
        LOG_ERROR << "Fail to select the vote users.....";
        return false;
    }

    auto& vote_counts       = vote_result.counts;
    auto& vote_items_status = vote_result.vote_items_status;
    auto  n_items           = cache_info.vote_item_ids.size();
    vote_counts.resize(n_items, 0);
    vote_items_status.resize(n_items, static_cast<uint8_t>(VoteItemStatus::kNotSelected));
    auto   vip_vote_counts    = vote_counts;
    size_t n_maximum_priority = 0;
    for (size_t i = 0; i < n_voter; ++i) {
        auto voter_id      = select_result[i].getValueOfId();
        auto vote_priority = select_result[i].getValueOfVotePriority();
        if (vote_priority == static_cast<uint8_t>(VotePriority::kInvalid)) {
            LOG_INFO << "voter id:" << voter_id
                     << " do not have the priority to vote!!!just ignore it!";
            continue;
        }

        auto& voter_info = cache_info.voter_infos.at(voter_id);
        // vip
        for (auto& choice : voter_info.choices) {
            vote_counts[choice]++;
        }
        if (vote_priority == static_cast<uint8_t>(VotePriority::kMaximum)) {
            ++n_maximum_priority;
            for (auto& choice : voter_info.choices) {
                vip_vote_counts[choice]++;
            }
        }
    }
    // the find the max stat
    auto& stat_vote_counts = n_maximum_priority > 0 ? vip_vote_counts : vote_counts;
    int   max_count        = 0;
    for (auto count : vote_counts) {
        max_count = max_count > count ? max_count : count;
    }
    if (max_count == 0) {
        LOG_ERROR << "all value is zero...";
        return false;
    }
    // maybe many max values!
    for (size_t i = 0; i < vote_counts.size(); ++i) {
        if (vote_counts[i] == max_count) {
            vote_items_status[i] = static_cast<uint8_t>(VoteItemStatus::kSelected);
        }
    }
    return true;
}


bool VoteController::call_when_vote_finished(uint32_t vote_id) {
    VoteResult            vote_result;
    std::vector<uint64_t> vote_item_ids;
    {
        std::lock_guard<std::mutex> guard(this->vote_mt);
        if (!this->vote_datas.contains(vote_id)) {
            LOG_WARN << "the vote id " << vote_id << " maybe removed...";
            return false;
        }
        LOG_INFO << "Remove vote id:" << vote_id << "from cache!";
        auto& cache_info = vote_datas.at(vote_id);
        if (!this->compute_vote_result(cache_info, vote_result)) {
            return false;
        }
        vote_item_ids = std::move(cache_info.vote_item_ids);
        // remove
        this->vote_datas.erase(vote_id);
    }

    auto db_client_ptr = app().getDbClient(get_db_client_name());
    if (db_client_ptr == nullptr) {
        LOG_ERROR << "Fail to get database ptr!";
        return false;
    }

    // 2.update vote finished!
    auto                  trans = db_client_ptr->newTransaction();
    orm::Mapper<VoteData> vote_data_mapper(trans);
    orm::Mapper<VoteItem> vote_item_mapper(trans);
    try {
        auto cond = orm::Criteria(VoteData::Cols::_vote_id, orm::CompareOperator::EQ, vote_id);
        vote_data_mapper.updateBy(
            {VoteData::Cols::_vote_status}, cond, static_cast<uint8_t>(VoteStatus::kFinished));
        // update the status
        for (size_t i = 0; i < vote_item_ids.size(); ++i) {
            auto cond =
                orm::Criteria(VoteItem::Cols::_id, orm::CompareOperator::EQ, vote_item_ids[i]);
            vote_item_mapper.updateBy(
                {VoteItem::Cols::_vote_item_count, VoteItem::Cols::_vote_item_status},
                cond,
                vote_result.counts[i],
                vote_result.vote_items_status[i]);
        }
    } catch (const orm::DrogonDbException& ex) {
        LOG_ERROR << ex.base().what();
        trans->rollback();
        return false;
    }
    // notify
    return true;
}


void VoteController::get_online_voters(const HttpRequestPtr&                         req,
                                       std::function<void(const HttpResponsePtr&)>&& callback) {
    auto db_client_ptr = app().getDbClient(get_db_client_name());
    if (db_client_ptr == nullptr) {
        LOG_ERROR << "Fail to get database ptr!";
        make_response_and_return(StatusCode::kInvalidDatabase, callback);
    }
    orm::Mapper<VoteUser> vote_data_mapper(db_client_ptr);
    try {
        auto online_voters = vote_data_mapper.findBy(orm::Criteria(
            VoteUser::Cols::_user_status, orm::CompareOperator::EQ, VoteUserStatus::kActive));
        if (online_voters.size() == 0) {
            LOG_WARN << "The num of online user is zero....";
        }
        auto        ret         = make_json_from_status_code(StatusCode::kSuccess);
        Json::Value voter_infos = Json::Value(Json::arrayValue);

        for (size_t i = 0; i < online_voters.size(); ++i) {
            Json::Value voter_info;
            voter_info["voter_name"] = online_voters[i].getValueOfUserName();
            voter_info["voter_id"]   = online_voters[i].getValueOfId();
            voter_infos.append(voter_info);
        }
        ret["voter_infos"] = voter_infos;
        auto resp          = HttpResponse::newHttpJsonResponse(ret);
        callback(resp);
    } catch (const orm::DrogonDbException& ex) {
        LOG_ERROR << ex.base().what();
        make_response_and_return(StatusCode::kSqlRuntimeError, callback);
    }
}


void VoteController::get_vote_num(const HttpRequestPtr&                         req,
                                  std::function<void(const HttpResponsePtr&)>&& callback) {
    auto json_data = req->jsonObject();
    if (json_data == nullptr) {
        make_response_and_return(StatusCode::kJsonParamIsNull, callback);
    }
    VoteNumReqParams params;
    if (auto ret = parse_get_vote_num_req(*json_data, params); ret != StatusCode::kSuccess) {
        make_response_and_return(ret, callback);
    }
    auto db_client_ptr = app().getDbClient(get_db_client_name());
    if (db_client_ptr == nullptr) {
        LOG_ERROR << "Fail to get database ptr!";
        make_response_and_return(StatusCode::kInvalidDatabase, callback);
    }

    orm::Mapper<VoteData> vote_data_mapper(db_client_ptr);
    try {
        auto cond =
            orm::Criteria(
                VoteData::Cols::_vote_creator_id, orm::CompareOperator::EQ, params.voter_id) &&
            orm::Criteria(
                VoteData::Cols::_vote_status, orm::CompareOperator::In, params.vote_status);
        auto count        = vote_data_mapper.count(cond

        );
        auto ret          = make_json_from_status_code(StatusCode::kSuccess);
        ret["vote_count"] = count;
        auto resp         = HttpResponse::newHttpJsonResponse(ret);
        callback(resp);
    } catch (const orm::DrogonDbException& ex) {
        LOG_ERROR << ex.base().what();
        make_response_and_return(StatusCode::kSqlRuntimeError, callback);
    }
}


void VoteController::get_chunk_vote_data(const HttpRequestPtr&                         req,
                                         std::function<void(const HttpResponsePtr&)>&& callback) {

    auto json_data = req->jsonObject();
    if (json_data == nullptr) {
        make_response_and_return(StatusCode::kJsonParamIsNull, callback);
    }
    ChunkVoteReqParams params;
    if (auto ret = parse_chunk_vote_data_req(*json_data, params); ret != StatusCode::kSuccess) {
        make_response_and_return(ret, callback);
    }


    auto db_client_ptr = app().getDbClient(get_db_client_name());
    if (db_client_ptr == nullptr) {
        LOG_ERROR << "Fail to get database ptr!";
        make_response_and_return(StatusCode::kInvalidDatabase, callback);
    }

    // 2.update vote finished!
    orm::Mapper<VoteData>            vote_data_mapper(db_client_ptr);
    orm::Mapper<VoteItem>            vote_item_mapper(db_client_ptr);
    orm::Mapper<VoteParticipateInfo> vote_participate_mapper(db_client_ptr);

    vote_data_mapper.limit(params.vote_num)
        .offset(params.vote_offset)
        .orderBy(VoteData::Cols::_vote_id, orm::SortOrder::DESC);
    auto        ret = make_json_from_status_code(StatusCode::kSuccess);
    Json::Value json_vote_datas(Json::arrayValue);
    try {
        auto results = vote_data_mapper.findBy(
            orm::Criteria(
                VoteData::Cols::_vote_creator_id, orm::CompareOperator::EQ, params.voter_id) &&
            orm::Criteria(
                VoteData::Cols::_vote_status, orm::CompareOperator::In, params.vote_status));
        if (results.size() == 0) {
            ret["vote_datas"] = json_vote_datas;
            auto resp         = HttpResponse::newHttpJsonResponse(ret);
            callback(resp);
            return;
        }
        orm::Mapper<VoteItem> vote_item_mapper(db_client_ptr);
        std::vector<uint32_t> vote_ids;
        vote_ids.reserve(results.size());
        for (size_t i = 0; i < results.size(); ++i) {
            // json_vote_datas.append(results[i].toJson());
            auto vote_id = results[i].getValueOfVoteId();
            vote_ids.push_back(vote_id);
        }

        std::vector<VoteItem> vote_items = vote_item_mapper.findBy(
            orm::Criteria(VoteItem::Cols::_vote_id, orm::CompareOperator::In, vote_ids));

        std::vector<VoteParticipateInfo> vote_participate_infos = vote_participate_mapper.findBy(
            orm::Criteria(VoteParticipateInfo::Cols::_vote_id, orm::CompareOperator::In, vote_ids));

        struct GroupValue {
            std::vector<size_t> item_indexes;
            std::vector<size_t> participate_indexes;
        };

        std::map<uint32_t, GroupValue> group_by_vote_ids;
        for (size_t i = 0; i < vote_items.size(); ++i) {
            auto  k = vote_items[i].getValueOfVoteId();
            auto& v = group_by_vote_ids[k].item_indexes;
            v.push_back(i);
        }

        for (size_t i = 0; i < vote_participate_infos.size(); ++i) {
            auto  k = vote_participate_infos[i].getValueOfVoteId();
            auto& v = group_by_vote_ids[k].participate_indexes;
            v.push_back(i);
        }

        for (size_t i = 0; i < results.size(); ++i) {
            auto     json_vote_data  = results[i].toJson();
            auto     json_vote_items = Json::Value(Json::arrayValue);
            auto     json_voters     = Json::Value(Json::arrayValue);
            auto     json_vote_names = Json::Value(Json::arrayValue);
            uint32_t vote_id         = results[i].getValueOfVoteId();
            if (group_by_vote_ids.contains(vote_id)) {
                auto& vv = group_by_vote_ids.at(vote_id);
                for (auto v : vv.item_indexes) {
                    json_vote_items.append(vote_items[v].getValueOfVoteItem());
                }
                for (auto v : vv.participate_indexes) {
                    auto participate_voter_id   = vote_participate_infos[v].getValueOfVoterId();
                    auto participate_voter_name = vote_participate_infos[v].getValueOfVoterName();
                    json_voters.append(participate_voter_id);
                    json_vote_names.append(participate_voter_name);
                }
            }
            json_vote_data["vote_items"]  = json_vote_items;
            json_vote_data["voters"]      = json_voters;
            json_vote_data["voter_names"] = json_vote_names;
            json_vote_datas.append(std::move(json_vote_data));
        }
        ret["vote_datas"] = json_vote_datas;
        auto resp         = HttpResponse::newHttpJsonResponse(ret);
        callback(resp);
    } catch (const orm::DrogonDbException& ex) {
        LOG_ERROR << ex.base().what();
        make_response_and_return(StatusCode::kSqlRuntimeError, callback);
    }
}

void VoteController::get_chunk_participate_vote_data(
    const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) {
    auto json_data = req->jsonObject();
    if (json_data == nullptr) {
        make_response_and_return(StatusCode::kJsonParamIsNull, callback);
    }
    ChunkParticipateVoteReqParams params;
    if (auto ret = parse_chunk_participate_vote_data_req(*json_data, params);
        ret != StatusCode::kSuccess) {
        make_response_and_return(ret, callback);
    }
    auto db_client_ptr = app().getDbClient(get_db_client_name());
    if (db_client_ptr == nullptr) {
        LOG_ERROR << "Fail to get database ptr!";
        make_response_and_return(StatusCode::kInvalidDatabase, callback);
    }


    LOG_INFO << "the select voter id is " << params.voter_id;
    orm::Mapper<VoteData> vote_data_mapper(db_client_ptr);
    vote_data_mapper.orderBy(VoteData::Cols::_vote_id, orm::SortOrder::DESC);
    orm::Mapper<VoteItem>            vote_item_mapper(db_client_ptr);
    orm::Mapper<VoteParticipateInfo> vote_participate_mapper(db_client_ptr);
    vote_participate_mapper.orderBy(VoteParticipateInfo::Cols::_vote_id, orm::SortOrder::DESC)
        .limit(params.vote_num)
        .offset(params.vote_offset);

    auto        ret = make_json_from_status_code(StatusCode::kSuccess);
    Json::Value json_vote_datas(Json::arrayValue);
    try {
        auto cond =
            orm::Criteria(
                VoteParticipateInfo::Cols::_voter_id, orm::CompareOperator::EQ, params.voter_id) &&
            orm::Criteria(VoteParticipateInfo::Cols::_vote_status,
                          orm::CompareOperator::In,
                          params.vote_status) &&
            orm::Criteria(VoteParticipateInfo::Cols::_vote_process_status,
                          orm::CompareOperator::In,
                          params.vote_process_status);

        auto related_vote_datas = vote_participate_mapper.findBy(cond);
        auto n                  = related_vote_datas.size();
        if (n == 0) {
            ret["vote_datas"] = json_vote_datas;
            auto resp         = HttpResponse::newHttpJsonResponse(ret);
            callback(resp);
            return;
        }

        std::vector<uint32_t>      vote_ids(n);
        std::map<uint32_t, size_t> vote_id_2_related;
        for (size_t i = 0; i < n; ++i) {
            auto vote_id               = related_vote_datas[i].getValueOfVoteId();
            vote_ids[i]                = vote_id;
            vote_id_2_related[vote_id] = i;
        }

        cond = orm::Criteria(VoteData::Cols::_vote_id, orm::CompareOperator::In, vote_ids);
        auto vote_datas = vote_data_mapper.findBy(cond);

        cond = orm::Criteria(VoteItem::Cols::_vote_id, orm::CompareOperator::In, vote_ids);
        std::vector<VoteItem>                   vote_items = vote_item_mapper.findBy(cond);
        std::map<uint32_t, std::vector<size_t>> group_by_vote_ids;
        for (size_t i = 0; i < vote_items.size(); ++i) {
            auto  k = vote_items[i].getValueOfVoteId();
            auto& v = group_by_vote_ids[k];
            v.push_back(i);
        }
        for (size_t i = 0; i < vote_datas.size(); ++i) {
            auto json_vote_data  = vote_datas[i].toJson();
            auto json_vote_items = Json::Value(Json::arrayValue);

            auto vote_id = vote_datas[i].getValueOfVoteId();
            LOG_INFO << vote_id;
            auto index = vote_id_2_related.at(vote_id);
            json_vote_data["vote_process_status"] =
                related_vote_datas[index].getValueOfVoteProcessStatus();

            if (group_by_vote_ids.contains(vote_id)) {
                auto& vv = group_by_vote_ids.at(vote_id);
                for (auto v : vv) {
                    json_vote_items.append(vote_items[v].getValueOfVoteItem());
                }
            }
            json_vote_data["vote_items"] = json_vote_items;
            json_vote_datas.append(json_vote_data);
        }

        ret["vote_datas"] = json_vote_datas;
        auto resp         = HttpResponse::newHttpJsonResponse(ret);
        callback(resp);
    } catch (const orm::DrogonDbException& ex) {
        LOG_ERROR << ex.base().what();
        make_response_and_return(StatusCode::kSqlRuntimeError, callback);
    }
}

void VoteController::get_participate_vote_num(
    const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) {
    auto json_data = req->jsonObject();
    if (json_data == nullptr) {
        make_response_and_return(StatusCode::kJsonParamIsNull, callback);
    }
    ParticipateVoteNumReqParams params;
    if (auto ret = parse_get_participate_vote_num_req(*json_data, params);
        ret != StatusCode::kSuccess) {
        make_response_and_return(ret, callback);
    }
    auto db_client_ptr = app().getDbClient(get_db_client_name());
    if (db_client_ptr == nullptr) {
        LOG_ERROR << "Fail to get database ptr!";
        make_response_and_return(StatusCode::kInvalidDatabase, callback);
    }

    orm::Mapper<VoteParticipateInfo> vote_mapper(db_client_ptr);
    LOG_INFO << "ggggggggggg" << params.voter_id;

    try {
        auto cond =
            orm::Criteria(
                VoteParticipateInfo::Cols::_voter_id, orm::CompareOperator::EQ, params.voter_id) &&
            orm::Criteria(VoteParticipateInfo::Cols::_vote_status,
                          orm::CompareOperator::In,
                          params.vote_status) &&
            orm::Criteria(VoteParticipateInfo::Cols::_vote_process_status,
                          orm::CompareOperator::In,
                          params.vote_process_status);
        auto count        = vote_mapper.count(cond);
        auto ret          = make_json_from_status_code(StatusCode::kSuccess);
        ret["vote_count"] = count;
        auto resp         = HttpResponse::newHttpJsonResponse(ret);
        callback(resp);

    } catch (const orm::DrogonDbException& ex) {
        LOG_ERROR << ex.base().what();
        make_response_and_return(StatusCode::kSqlRuntimeError, callback);
    }
}