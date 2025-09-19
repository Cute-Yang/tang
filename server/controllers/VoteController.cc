#include "VoteController.h"
#include "../models/TestVotes.h"
#include "util.h"
#include <mutex>

// Add definition of your processing function here
using namespace tang::common;
using namespace tang::server::utils;

using VoteData = drogon_model::vote::TestVotes;

// the keys!!!
constexpr const char* vote_topic_key = "vote_topic";
constexpr const char* vote_items_key = "vote_items";
// maybe used in the future!
constexpr const char*                           vote_creator_id_key  = "vote_creator_id";
constexpr const char*                           vote_creator_key     = "vote_creator";
constexpr const char*                           vote_create_time_key = "vote_create_time";
constexpr const char*                           voters_key           = "voters";
constexpr const char*                           is_single_choice_key = "is_single_choice";
constexpr size_t                                vote_key_num         = 7;
constexpr std::array<const char*, vote_key_num> json_keys            = {vote_topic_key,
                                                                        vote_items_key,
                                                                        vote_creator_id_key,
                                                                        vote_creator_key,
                                                                        vote_create_time_key,
                                                                        voters_key,
                                                                        is_single_choice_key};

bool validate_vote_data(const Json::Value&                                 json_data,
                        const std::function<void(const HttpResponsePtr&)>& callback) {
    // validate whether the key exist!
    for (size_t i = 0; i < json_keys.size(); ++i) {
        if (!json_data.isMember(json_keys[i])) {
            LOG_ERROR << "Missing json key:" << json_keys[i];
            make_response_and_return(StatusCode::kJsonKeyError, callback, false);
        }
    }
    // validate the type!
    if (!json_data[vote_topic_key].isString()) {
        make_response_and_return(StatusCode::kJsonTypeError, callback, false);
    }

    if (!json_data[vote_items_key].isArray()) {
        make_response_and_return(StatusCode::kJsonTypeError, callback, false);
    }

    for (auto& item : json_data[vote_items_key]) {
        if (!item.isString()) {
            make_response_and_return(StatusCode::kJsonTypeError, callback, false);
        }
    }

    if (!json_data[vote_creator_key].isString()) {
        make_response_and_return(StatusCode::kJsonTypeError, callback, false);
    }

    if (!json_data[vote_create_time_key.isString]) {
        make_response_and_return(StatusCode::kJsonTypeError, callback, false);
    }

    if (!json_data[voters_key].isArray()) {
        make_response_and_return(StatusCode::kJsonTypeError, callback, false);
    }

    if (json_data[vote_items_key].size() != json_data[voters_key].size()) {
        make_response_and_return(StatusCode::kArraySizeNotMatchError, callback, false);
    }

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
    if (!validate_vote_data(*json_data, callback)) {
        return;
    }

    // then parse the json
    ParsedVoteData parsed_vote_data;
    parsed_vote_data.is_valid = true;
    try {
        // vote topic
        parsed_vote_data.vote_topic = (*json_data)[vote_topic_key].asString();
        parsed_vote_data.vote_items.reserve((*json_data)[vote_items_key].size());
        for (auto& item : (*json_data)[vote_items_key]) {
            parsed_vote_data.vote_items.push_back(item.asString());
        }
        parsed_vote_data.vote_creator     = (*json_data)[vote_creator_key].asString();
        parsed_vote_data.vote_create_time = (*json_data)[vote_create_time_key].asInt64();
        parsed_vote_data.voters.reserve((*json_data)[voters_key].size());
        for (auto& item : (*json_data)[voters_key]) {
            parsed_vote_data.voters.push_back(item.asInt());
        }

        parsed_vote_data.vote_creator_id  = (*json_data)[vote_creator_id_key].asInt();
        parsed_vote_data.is_single_choice = (*json_data)[is_single_choice_key].asBool();

    } catch (Json::Exception& ex) {
        LOG_ERROR << "Json exxception:" << ex.what();
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
    insert_vote_data.setVoteTopic(parsed_vote_data.vote_topic);
    insert_vote_data.setVoteCreator(parsed_vote_data.vote_creator);
    // hei,using the global
    // make sure the microsecons!!!
    insert_vote_data.setVoteCreateTime(::trantor::Date(parsed_vote_data.vote_create_time));
    insert_vote_data.setVoteType(parsed_vote_data.is_single_choice ? 0 : 1);
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
        auto guard    = std::lock_guard<std::mutex>(this->vote_mt);
        value.vote_id = vote_id;
        this->vote_datas.insert({vote_id, std::move(value)});
        LOG_INFO << "Suucessfully insert vote:" << vote_id;
        // here should return a vote_id?
        make_response_and_return(StatusCode::kSuccess, callback);
    };

    auto c2 = [callback](const orm::DrogonDbException& ex) {
        LOG_ERROR << "Fail to insert data with error:" << ex.base().what();
        make_response_and_return(StatusCode::kSqlRuntimeError, callback);
    };
    vote_data_mapper.insert(insert_vote_data, c1, c2);
}


void VoteController::remove_vote(const HttpRequestPtr&                         req,
                                 std::function<void(const HttpResponsePtr&)>&& callback) {
    auto vote_id_str = req->getParameter("vote_id");
    if (vote_id_str.empty()) {
        make_response_and_return(StatusCode::kInvalidVoteid, callback);
    }
    // then convert to the int32_t
    uint32_t vote_id;

    // got the error!
    auto result =
        std::from_chars(vote_id_str.data(), vote_id_str.data() + vote_id_str.size(), vote_id);
    if (result.ec == std::errc()) {
        LOG_ERROR << "Fail to convert vote_id:" << vote_id_str << " to int32_t";
        make_response_and_return(StatusCode::kInvalidVoteid, callback);
    }

    auto guard = std::lock_guard<std::mutex>(this->vote_mt);
    auto it    = this->vote_datas.find(vote_id);
    if (it == this->vote_datas.end()) {
        LOG_ERROR << "Fail to find vote_id:" << vote_id << " in the memory!";
        make_response_and_return(StatusCode::kInvalidVoteid, callback);
    }
    this->vote_datas.erase(it);
    LOG_INFO << "Successfully remove vote_id:" << vote_id << " from memory!";

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

    };
    auto c2 = [callback](const orm::DrogonDbException& ex) {
        LOG_ERROR << "sql error:" << ex.base().what();
        make_response_from_status_code(StatusCode::kSqlRuntimeError, callback);
    };

    // vote_data_mapper.updateBy(
    //     {VoteData::Cols::_is_valid},
    //     c1,
    //     c2,
    //     orm::Criteria(VoteData::Cols::_vote_id, orm::CompareOperator::EQ, vote_id),
    //     0);

}
