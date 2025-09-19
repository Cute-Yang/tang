#include "VoteController.h"
#include "../models/TestVotes.h"
#include "util.h"


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
    try {
        // vote topic
        parsed_vote_data.vote_topic = (*json_data)[vote_topic_key].asString();
        parsed_vote_data.vote_items.reserve((*json_data)[vote_items_key].size());
        for (auto& item : (*json_data)[vote_items_key]) {
            parsed_vote_data.vote_items.push_back(item.asString());
        }
        parsed_vote_data.vote_creator     = (*json_data)[vote_creator_key].asString();
        parsed_vote_data.vote_create_time = (*json_data)[vote_create_time_key].asString();
        parsed_vote_data.voters.reserve((*json_data)[voters_key].size());
        for (auto& item : (*json_data)[voters_key]) {
            parsed_vote_data.voters.push_back(item.asInt());
        }

        parsed_vote_data.vote_creator_id = (*json_data)[vote_creator_id_key].asInt();


    } catch (Json::Exception& ex) {
        LOG_ERROR << "Json exxception:" << ex.what();
    }

    // need to insert to sql
}