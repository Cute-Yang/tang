#include "VoteController.h"
#include "../models/TestVoteData.h"
#include "../models/TestVoteItem.h"
#include "../models/TestVoteUser.h"
#include "util.h"
#include <mutex>


// Add definition of your processing function here
using namespace tang::common;
using namespace tang::server::utils;

using VoteData = drogon_model::vote::TestVoteData;
using VoteUser = drogon_model::vote::TestVoteUser;
using VoteItem = drogon_model::vote::TestVoteItem;


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

// make sure ends with \0 for some C function!
struct SendVoteChoiceJsonValidator {
    static constexpr const char* user_id_key      = "user_id";
    static constexpr const char* user_name_key    = "user_name";
    static constexpr const char* vote_choices_key = "vote_choices";
    // important!
    static constexpr const char* vote_id_key = "vote_id";

    static constexpr size_t                           key_num   = 4;
    static constexpr std::array<const char*, key_num> json_keys = {
        user_id_key, user_name_key, vote_choices_key, vote_id_key};
    static StatusCode validate_vote_data(const Json::Value& json_data) {

        for (size_t i = 0; i < json_keys.size(); ++i) {
            if (!json_data.isMember(json_keys[i])) {
                {
                    LOG_ERROR << "Missing expected json:" << json_keys[i];
                    return StatusCode::kJsonKeyError;
                }
            }
        }

        // valide the type!
        if (!json_data[user_id_key].isInt()) {
            return StatusCode::kJsonTypeError;
        }

        if (!json_data[user_name_key].isString()) {
            return StatusCode::kJsonTypeError;
        }

        if (!json_data[vote_choices_key].isArray()) {
            return StatusCode::kJsonTypeError;
        }

        for (auto& item : json_data[vote_choices_key]) {
            if (!item.isInt()) {
                return StatusCode::kJsonTypeError;
            }
        }

        if (!json_data[vote_id_key].isInt()) {
            return StatusCode::kJsonTypeError;
        }

        return StatusCode::kSuccess;
    }
};


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

    // then parse the json
    VoteCacheData parsed_vote_data;
    parsed_vote_data.vote_status = VoteStatus::kReady;
    try {
        // vote topic
        parsed_vote_data.vote_topic = (*json_data)[VoteJsonValidator::vote_topic_key].asString();
        auto n_item                 = (*json_data)[VoteJsonValidator::vote_items_key].size();
        parsed_vote_data.vote_items.reserve(n_item);
        for (auto& item : (*json_data)[VoteJsonValidator::vote_items_key]) {
            parsed_vote_data.vote_items.push_back(item.asString());
        }
        parsed_vote_data.vote_creator =
            (*json_data)[VoteJsonValidator::vote_creator_key].asString();
        parsed_vote_data.vote_create_time =
            (*json_data)[VoteJsonValidator::vote_create_time_key].asInt64();
        for (auto& item : (*json_data)[VoteJsonValidator::voters_key]) {
            voter_id_type voter_id = item.asUInt();
            parsed_vote_data.voter_choices.insert({voter_id, {}});
        }

        parsed_vote_data.vote_creator_id =
            (*json_data)[VoteJsonValidator::vote_creator_id_key].asInt();
        auto choice_type_value = (*json_data)[VoteJsonValidator::vote_choice_type_key].asInt();
        if (choice_type_value >= vote_choice_type_count) {
            LOG_ERROR << "Invalid choice type:" << choice_type_value;
            make_response_and_return(StatusCode::kInvalidChoiceType, callback);
        }

        parsed_vote_data.choice_type = static_cast<VoteChoiceType>(choice_type_value);


    } catch (Json::Exception& ex) {
        LOG_ERROR << "Json exxception:" << ex.what();
        make_response_and_return(StatusCode::kParseJsonError, callback);
    }

    // need to insert to sql

    auto db_client_ptr = app().getDbClient(get_db_client_name());
    if (db_client_ptr == nullptr) {
        // wow,the databas is nullptr!
        // here we do not need the trans
        LOG_ERROR << "database ptr is invalid...";
        make_response_and_return(StatusCode::kInvalidDatabase, callback);
    }
    orm::Mapper<VoteData> vote_data_mapper(db_client_ptr);
    VoteData              insert_vote_data;
    // vote_data_mapper.
    insert_vote_data.setVoteTopic(parsed_vote_data.vote_topic);
    insert_vote_data.setVoteCreator(parsed_vote_data.vote_creator);
    insert_vote_data.setVoteCreatorId(parsed_vote_data.vote_creator_id),
        // hei,using the global
        // make sure the microsecons!!!
    insert_vote_data.setVoteCreateTime(::trantor::Date(parsed_vote_data.vote_create_time));
    insert_vote_data.setVoteType(static_cast<uint8_t>(parsed_vote_data.choice_type));
    // very important!
    //  insert_vote_data.setValid(parsed_vote_data.is_valid);
    //  very important,the name maybe changed!
    //   insert_vote_data.setVoteCreatorId(parsed_vote_data.vote_creator_id)
    auto c1 = [callback, value = std::move(parsed_vote_data), this](VoteData vote_data) mutable {
        // auto with decay type!
        auto        vote_id    = vote_data.getValueOfVoteId();
        const auto& vote_topic = value.vote_topic;
        LOG_INFO << "Successfully add vote, vote_id:" << vote_id << " vote_topic:" << vote_topic;
        // just add to the map!
        // then insert the data!!!
        value.vote_id = vote_id;

        if (auto ret = this->add_vote_data_to_cache(std::move(value));
            ret != StatusCode::kSuccess) {
            make_response_and_return(ret, callback);
            // should reset status -> invalid???
        }
        // here should return a vote_id?
        auto ret       = make_json_from_status_code(StatusCode::kSuccess);
        ret["vote_id"] = vote_id;
        auto resp      = HttpResponse::newHttpJsonResponse(ret);
        callback(resp);
        // then notify all user!
    };

    auto c2 = [callback](const orm::DrogonDbException& ex) {
        LOG_ERROR << "Fail to insert data with error:" << ex.base().what();
        make_response_and_return(StatusCode::kSqlRuntimeError, callback);
    };
    vote_data_mapper.insert(insert_vote_data, c1, c2);
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

void VoteController::send_vote_choices(const HttpRequestPtr&                         req,
                                       std::function<void(const HttpResponsePtr&)>&& callback) {
    auto json_data = req->getJsonObject();
    if (json_data == nullptr) {
        LOG_ERROR << "Fail to get json data";
        make_response_and_return(StatusCode::kJsonParamIsNull, callback);
    }
    // valid data the json
    if (auto ret = SendVoteChoiceJsonValidator::validate_vote_data(*json_data);
        ret != StatusCode::kSuccess) {
        make_response_and_return(ret, callback);
    }

    std::vector<int> vote_choices;
    vote_id_type     vote_id;
    voter_id_type    voter_id;
    try {
        // std::string user_name =
        // (*json_data)[SendVoteChoiceJsonValidator::user_name_key].asString();
        voter_id = (*json_data)[SendVoteChoiceJsonValidator::user_id_key].asUInt();

        vote_id = (*json_data)[SendVoteChoiceJsonValidator::vote_id_key].asInt();
        if (vote_id <= 0) {
            LOG_ERROR << "invalid vote id:" << vote_id;
            make_response_and_return(StatusCode::kInvalidVoteid, callback);
        }

        auto n_choice = (*json_data)[SendVoteChoiceJsonValidator::vote_choices_key].size();
        if (n_choice == 0) {
            LOG_ERROR << "empty choices for vote_id:" << vote_id << "???";
            make_response_and_return(StatusCode::kEmptyVoteChoice, callback);
        }

        vote_choices.reserve(n_choice);
        for (auto& item : (*json_data)[SendVoteChoiceJsonValidator::vote_choices_key]) {
            vote_choices.push_back(item.asInt());
        }

    } catch (const Json::Exception& ex) {
        LOG_ERROR << "Json parse error:" << ex.what();
        make_response_and_return(StatusCode::kParseJsonError, callback);
    }

    bool vote_is_finished = false;
    // add the count
    {
        std::lock_guard<std::mutex> guard(this->vote_mt);
        if (!vote_datas.contains(vote_id)) {
            LOG_ERROR << "The vote_id:" << vote_id << " is not exist!";
            make_response_and_return(StatusCode::KVoteIdNotFound, callback);
        }
        auto& vote_data = vote_datas[vote_id];
        if (vote_data.choice_type == VoteChoiceType::kSingleChoice && vote_choices.size() != 1) {
            LOG_ERROR << "The vote data,vote_id:" << vote_id
                      << " vote_topic:" << vote_data.vote_topic
                      << " is single choice type,but get multi choices which is unexpeced!";
            make_response_and_return(StatusCode::kVoteChoceTypeMismatch, callback);
        }
        int n_vote_item = vote_data.vote_items.size();
        for (auto choice : vote_choices) {
            if (choice < 0 || choice >= n_vote_item) {
                make_response_and_return(StatusCode::kInvalidVoteChoice, callback);
            }
        }
        if (!vote_data.voter_choices.contains(voter_id)) {
            LOG_ERROR << "the voter is not valid!";
            make_response_and_return(StatusCode::kVoterIsNotInclude, callback);
        }
        vote_data.voter_choices[voter_id] = std::move(vote_choices);
        vote_data.n_finished_voter++;
        vote_is_finished = (vote_data.n_finished_voter == vote_data.voter_choices.size());
    }
    make_response_only(StatusCode::kSuccess, callback);

    if (vote_is_finished) {
        this->vote_finished_work(vote_id);
    }
}


StatusCode VoteController::add_vote_data_to_cache(
    VoteController::VoteCacheData&& vote_data) noexcept {
    auto guard = std::lock_guard<std::mutex>(this->vote_mt);

    if (this->vote_datas.size() >= VoteCacheConfig::max_vote_cache_num) {
        LOG_ERROR << "The vote cache is full,now can not add any vote data to it!";
        return StatusCode::kVoteCacheIsFull;
    }
    if (!this->vote_datas.insert({vote_data.vote_id, std::move(vote_data)}).second) {
        LOG_ERROR << "Fail to insert vote:" << vote_data.vote_id;
        return StatusCode::kFailAddVoteToCache;
    }
    LOG_INFO << "Sucessfully insert vote:" << vote_data.vote_id
             << " topic:" << vote_data.vote_topic;
    return StatusCode::kSuccess;
}

StatusCode VoteController::remove_vote_data_from_cache(vote_id_type vote_id) noexcept {
    std::lock_guard<std::mutex> guard(this->vote_mt);
    if (!vote_datas.contains(vote_id)) {
        LOG_ERROR << "Can not remove vote_id:" << vote_id << " which is not exist!";
        return StatusCode::KVoteIdNotFound;
    }
    vote_datas.erase(vote_id);
    return StatusCode::kSuccess;
}

VoteController::VoteResult VoteController::compute_vote_result(
    const VoteCacheData& vote_data) const noexcept {
    VoteResult vote_result;

    auto db_client_ptr = app().getDbClient(get_db_client_name());
    if (db_client_ptr == nullptr) {
        LOG_ERROR << "the db client is nullptr!";
        return vote_result;
    }
    auto n_voter = vote_data.voter_choices.size();
    assert(n_voter > 0);
    std::vector<voter_id_type> voter_ids;
    voter_ids.reserve(n_voter);
    for (auto& [k, _] : vote_data.voter_choices) {
        voter_ids.push_back(k);
    }

    orm::Mapper<VoteUser> user_mapper(db_client_ptr);
    std::vector<VoteUser> select_result;
    try {
        select_result = user_mapper.findBy(
            orm::Criteria(VoteUser::Cols::_id, orm::CompareOperator::In, voter_ids));
    } catch (const orm::DrogonDbException& ex) {
        LOG_ERROR << ex.base().what();
        return vote_result;
    }
    if (select_result.size() != n_voter) {
        LOG_ERROR << "Fail to select the vote users.....";
        return vote_result;
    }

    auto& vote_counts = vote_result.counts;
    vote_counts.resize(vote_data.vote_items.size());
    size_t        n_maximum_priority = 0;
    voter_id_type vip_voter_id       = -1;
    for (size_t i = 0; i < n_voter; ++i) {
        auto voter_id      = select_result[i].getValueOfId();
        auto vote_priority = select_result[i].getValueOfVotePriority();
        if (vote_priority == static_cast<uint8_t>(VotePriority::kInvalid)) {
            LOG_INFO << "voter id:" << " do not have the priority to vote!!!just ignore it!";
            continue;
        }
        if (vote_priority == static_cast<uint8_t>(VotePriority::kMaximum)) {
            ++n_maximum_priority;
            vip_voter_id = voter_id;
        }
        auto& voter_choices = vote_data.voter_choices.at(voter_id);
        for (auto& choice : voter_choices) {
            vote_counts[choice]++;
        }
    }

    if (n_maximum_priority > 1) {
        LOG_ERROR << "only allow one person have the maximum priority!";
        return vote_result;
    } else if (n_maximum_priority == 1) {
        LOG_INFO << "here has a vip...";
        return vote_result;
    }

    // the find the most!!!
    int max_count = 0;
    for (auto count : vote_counts) {
        max_count = max_count > count ? count : max_count;
    }
    if (max_count == 0) {
        LOG_ERROR << "wow,no people...";
        return {};
    }

    auto& ret = vote_result.ret;
    for (size_t i = 0; i < vote_counts.size(); ++i) {
        if (vote_counts[i] == max_count) {
            ret.push_back(i);
        }
    }

    return vote_result;
}

bool VoteController::vote_finished_work(vote_id_type vote_id) {
    VoteResult               vote_result;
    std::vector<std::string> vote_items;
    {
        // 1.compute and remove from cache!
        std::lock_guard<std::mutex> guard(this->vote_mt);
        if (!this->vote_datas.contains(vote_id)) {
            return true;
        }
        LOG_INFO << "Remove vote id:" << vote_id << "from cache!";
        auto& vote_data = vote_datas.at(vote_id);
        vote_result     = this->compute_vote_result(vote_data);
        vote_items      = std::move(vote_data.vote_items);
        vote_id         = vote_data.vote_id;
        // remove
        this->vote_datas.erase(vote_id);
    }

    auto db_client_ptr = app().getDbClient(get_db_client_name());
    if (db_client_ptr == nullptr) {
        LOG_ERROR << "Fail to get database ptr!";
        return false;
    }

    // 2.update vote finished!
    orm::Mapper<VoteData> vote_data_mapper(db_client_ptr);
    try {
        auto n = vote_data_mapper.updateBy(
            {VoteData::Cols::_vote_status},
            orm::Criteria(VoteData::Cols::_vote_id, orm::CompareOperator::EQ, vote_id),
            static_cast<uint8_t>(VoteStatus::kFinished));
        if (n != 1) {
            LOG_ERROR << "fail to update vote...";
            return false;
        }
    } catch (const orm::DrogonDbException& ex) {
        LOG_ERROR << ex.base().what();
        return false;
    }

    // 3.insert to database

    orm::Mapper<VoteItem> vote_item_mapper(db_client_ptr);
    VoteItem              vote_item;
    try {
        for (size_t i = 0; i < vote_items.size(); ++i) {
            vote_item.setVoteId(vote_id);
            vote_item.setVoteItem(std::move(vote_items[i]));
            vote_item.setVoteCount(vote_result.counts[i]);
            vote_item_mapper.insert(vote_item);
            LOG_INFO << "add vote item,id:" << vote_item.getValueOfVoteId();
        }
    } catch (const orm::DrogonDbException& ex) {
        LOG_ERROR << "fail to insert vote items...";
        return false;
    }

    // 4.notify voters

    return true;
}


void VoteController::get_online_voters(const HttpRequestPtr&                         req,
                                       std::function<void(const HttpResponsePtr&)>&& callback) {
    auto db_client_ptr = app().getDbClient(get_db_client_name());
    if (db_client_ptr == nullptr) {
        LOG_ERROR << "Fail to get database ptr!";
        make_response_and_return(StatusCode::kInvalidDatabase, callback);
    }

    // 2.update vote finished!
    orm::Mapper<VoteUser> vote_data_mapper(db_client_ptr);
    auto                  online_voters = vote_data_mapper.findBy(orm::Criteria(
        VoteUser::Cols::_user_status, orm::CompareOperator::EQ, VoteUserStatus::kActive));
    if (online_voters.size() == 0) {
        LOG_WARN << "The num of online user is zero....";
    }

    auto ret = make_json_from_status_code(StatusCode::kSuccess);

    Json::Value voter_infos = Json::Value(Json::arrayValue);

    for (size_t i = 0; i < ret.size(); ++i) {
        Json::Value voter_info;
        voter_info["voter_name"] = online_voters[i].getValueOfUserName();
        voter_info["voter_id"]   = online_voters[i].getValueOfId();
        voter_infos.append(voter_info);
    }
    ret["voter_infos"] = voter_infos;
    auto resp          = HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}


void VoteController::get_vote_num(const HttpRequestPtr&                         req,
                                  std::function<void(const HttpResponsePtr&)>&& callback) {
    std::string voter_id_str = req->getParameter("voter_id");
    uint32_t    voter_id;
    auto        cvt =
        std::from_chars(voter_id_str.data(), voter_id_str.data() + voter_id_str.size(), voter_id);

    if (cvt.ec != std::errc()) {
        LOG_ERROR << "invlaid voter id str " << voter_id_str;
        make_response_and_return(StatusCode::kUserisInvalid, callback);
    }
    auto db_client_ptr = app().getDbClient(get_db_client_name());
    if (db_client_ptr == nullptr) {
        LOG_ERROR << "Fail to get database ptr!";
        make_response_and_return(StatusCode::kInvalidDatabase, callback);
    }

    // 2.update vote finished!
    orm::Mapper<VoteData> vote_data_mapper(db_client_ptr);

    auto c1 = [callback](size_t count) {
        auto ret          = make_json_from_status_code(StatusCode::kSuccess);
        ret["vote_count"] = count;
        auto resp         = HttpResponse::newHttpJsonResponse(ret);
        callback(resp);
    };

    auto c2 = [callback](const orm::DrogonDbException& ex) {
        make_response_and_return(StatusCode::kSqlRuntimeError, callback);
    };


    vote_data_mapper.count(
        orm::Criteria(VoteData::Cols::_vote_creator_id, orm::CompareOperator::EQ, voter_id),
        c1,
        c2);
}


void VoteController::get_chunk_vote_data(const HttpRequestPtr&                         req,
                                         std::function<void(const HttpResponsePtr&)>&& callback) {
    std::string vote_num_str    = req->getParameter("vote_num");
    std::string vote_offset_str = req->getParameter("vote_offset");
    std::string voter_id_str    = req->getParameter("voter_id");
    uint32_t    voter_id;

    uint32_t vote_num;
    uint32_t vote_offset;
    auto     cvt =
        std::from_chars(voter_id_str.data(), voter_id_str.data() + voter_id_str.size(), voter_id);

    if (cvt.ec != std::errc()) {
        LOG_ERROR << "invlaid voter id str " << voter_id_str;
        make_response_and_return(StatusCode::kUserisInvalid, callback);
    }
    cvt = std::from_chars(vote_num_str.data(), vote_num_str.data() + vote_num_str.size(), vote_num);

    if (cvt.ec != std::errc()) {
        LOG_ERROR << "invlaid voter id str " << vote_num_str;
        make_response_and_return(StatusCode::kInvalidNumberError, callback);
    }
    cvt = std::from_chars(
        vote_offset_str.data(), vote_offset_str.data() + vote_offset_str.size(), vote_offset);

    if (cvt.ec != std::errc()) {
        LOG_ERROR << "invlaid voter id str " << vote_offset_str;
        make_response_and_return(StatusCode::kInvalidNumberError, callback);
    }
    auto db_client_ptr = app().getDbClient(get_db_client_name());
    if (db_client_ptr == nullptr) {
        LOG_ERROR << "Fail to get database ptr!";
        make_response_and_return(StatusCode::kInvalidDatabase, callback);
    }

    // 2.update vote finished!
    orm::Mapper<VoteData> vote_data_mapper(db_client_ptr);

    orm::Mapper<VoteItem> vote_item_mapper(db_client_ptr);

    vote_data_mapper.limit(vote_num)
        .offset(vote_offset)
        .orderBy(VoteData::Cols::_vote_id, orm::SortOrder::DESC);
    try {
        auto results = vote_data_mapper.findBy(
            orm::Criteria(VoteData::Cols::_vote_creator_id, orm::CompareOperator::EQ, voter_id));

        orm::Mapper<VoteItem> vote_item_mapper(db_client_ptr);
        std::vector<uint32_t> vote_ids;
        auto                  ret = make_json_from_status_code(StatusCode::kSuccess);
        Json::Value           json_vote_datas(Json::arrayValue);
        for (size_t i = 0; i < results.size(); ++i) {
            // json_vote_datas.append(results[i].toJson());
            vote_ids.push_back(results[i].getValueOfVoteId());
        }
        try {
            std::vector<VoteItem> vote_items = vote_item_mapper.findBy(
                orm::Criteria(VoteItem::Cols::_vote_id, orm::CompareOperator::In, vote_ids));
            std::map<uint32_t, std::vector<size_t>> group_by_vote_ids;
            for (size_t i = 0; i < vote_items.size(); ++i) {
                group_by_vote_ids[vote_items[i].getValueOfVoteId()].push_back(i);
            }

            for (size_t i = 0; i < results.size(); ++i) {
                auto     json_vote_data  = results[i].toJson();
                auto     json_vote_items = Json::Value(Json::arrayValue);
                uint32_t vote_id         = results[i].getValueOfVoteId();
                if (group_by_vote_ids.contains(vote_id)) {
                    auto& indexes = group_by_vote_ids.at(vote_id);
                    for (auto index : indexes) {
                        json_vote_items.append(vote_items[index].getValueOfVoteItem());
                    }
                }
                json_vote_data["vote_items"] = json_vote_items;
                json_vote_datas.append(std::move(json_vote_data));
            }
            ret["vote_datas"] = json_vote_datas;
        } catch (const orm::DrogonDbException& ex) {
            LOG_ERROR << ex.base().what();
            make_response_and_return(StatusCode::kSqlRuntimeError, callback);
        }

        auto resp = HttpResponse::newHttpJsonResponse(ret);
        callback(resp);
    } catch (const orm::DrogonDbException& ex) {
        LOG_ERROR << ex.base().what();
        make_response_and_return(StatusCode::kSqlRuntimeError, callback);
    }
}