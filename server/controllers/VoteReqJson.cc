#include "VoteReqJson.h"
#include "common/http_json_keys.h"
// #include "json/json.h"

using namespace tang::common;
namespace tang {
namespace server {

common::StatusCode parse_chunk_vote_data_req(const Json::Value&  json_data,
                                             ChunkVoteReqParams& params) {
    auto keys = GetChunkVoteReqKeys::keys;
    for (auto key : keys) {
        if (!json_data.isMember(key)) {
            return StatusCode::kJsonKeyError;
        }
    }
    if (!json_data[GetChunkVoteReqKeys::voter_id_key].isUInt()) {
        return StatusCode::kJsonTypeError;
    }
    if (!json_data[GetChunkVoteReqKeys::vote_num_key].isUInt()) {
        return StatusCode::kJsonTypeError;
    }
    if (!json_data[GetChunkVoteReqKeys::vote_offset_key].isUInt()) {
        return StatusCode::kJsonTypeError;
    }

    if (!json_data[GetChunkVoteReqKeys::vote_status_key].isArray()) {
        return StatusCode::kJsonTypeError;
    }
    for (auto s : json_data[GetChunkVoteReqKeys::vote_status_key]) {
        if (!s.isUInt()) {
            {
                return StatusCode::kJsonTypeError;
            }
        }
    }

    params.voter_id        = json_data[GetChunkVoteReqKeys::voter_id_key].asUInt();
    params.vote_num        = json_data[GetChunkVoteReqKeys::vote_num_key].asUInt();
    params.vote_offset     = json_data[GetChunkVoteReqKeys::vote_offset_key].asUInt();
    auto& vote_status      = params.vote_status;
    auto& json_vote_status = json_data[GetChunkVoteReqKeys::vote_status_key];
    auto  n                = json_vote_status.size();
    auto  cc               = static_cast<decltype(n)>(VoteStatus::count);
    if (n > cc || n == 0) {
        return StatusCode::kLogicError;
    }
    vote_status.resize(n);
    for (int i = 0; i < json_vote_status.size(); ++i) {
        auto v = json_vote_status[i].asUInt();
        if (v >= cc) {
            return StatusCode::kLogicError;
        }
        vote_status[i] = v;
    }
    return StatusCode::kSuccess;
}

common::StatusCode parse_chunk_participate_vote_data_req(const Json::Value&             json_data,
                                                         ChunkParticipateVoteReqParams& params) {
    if (auto ret = parse_chunk_vote_data_req(json_data, static_cast<ChunkVoteReqParams&>(params));
        ret != StatusCode::kSuccess) {
        return ret;
    }
    constexpr auto k = GetParticipateVoteReqKeys::vote_process_status_key;
    if (!json_data.isMember(k)) {
        return StatusCode::kJsonKeyError;
    }
    if (!json_data[k].isArray()) {
        return StatusCode::kJsonTypeError;
    }
    auto json_process_status = json_data[k];
    auto n                   = json_process_status.size();
    auto cc                  = static_cast<size_t>(VoteProcessStatus::count);
    if (n == 0 || n > cc) {
        return StatusCode::kLogicError;
    }
    auto& process_status = params.vote_process_status;
    process_status.resize(n);
    for (int i = 0; i < n; ++i) {
        if (!json_process_status[i].isUInt()) {
            return StatusCode::kJsonTypeError;
        }
        auto v = json_process_status[i].asUInt();
        if (v >= cc) {
            return StatusCode::kLogicError;
        }
        process_status[i] = v;
    }
    return StatusCode::kSuccess;
}

common::StatusCode parse_get_vote_num_req(const Json::Value& json_data, VoteNumReqParams& params) {
    auto keys = GetVoteNumReqKeys::keys;
    for (auto key : keys) {
        if (!json_data.isMember(key)) {
            return StatusCode::kJsonKeyError;
        }
    }

    if (!json_data[GetVoteNumReqKeys::voter_id_key].isUInt()) {
        return StatusCode::kJsonTypeError;
    }

    if (!json_data[GetVoteNumReqKeys::vote_status_key].isArray()) {
        return StatusCode::kJsonTypeError;
    }

    for (auto s : json_data[GetVoteNumReqKeys::vote_status_key]) {
        if (!s.isUInt()) {
            {
                return StatusCode::kJsonTypeError;
            }
        }
    }
    params.voter_id        = json_data[GetVoteNumReqKeys::voter_id_key].asUInt();
    auto& json_vote_status = json_data[GetVoteNumReqKeys::vote_status_key];
    auto  n                = json_vote_status.size();
    auto  cc               = static_cast<decltype(n)>(VoteStatus::count);
    if (n > cc || n == 0) {
        return StatusCode::kLogicError;
    }

    auto& vote_status = params.vote_status;
    vote_status.resize(n);
    for (int i = 0; i < json_vote_status.size(); ++i) {
        auto v = json_vote_status[i].asUInt();
        if (v >= cc) {
            return StatusCode::kLogicError;
        }
        vote_status[i] = v;
    }
    return StatusCode::kSuccess;
}


common::StatusCode parse_get_participate_vote_num_req(const Json::Value&           json_data,
                                                      ParticipateVoteNumReqParams& params) {
    if (auto ret = parse_get_vote_num_req(json_data, static_cast<VoteNumReqParams&>(params));
        ret != StatusCode::kSuccess) {
        return ret;
    }
    auto k = GetParticipateNumReqKeys::vote_process_status_key;
    if (!json_data.isMember(k)) {
        return StatusCode::kJsonKeyError;
    }
    if (!json_data[k].isArray()) {
        return StatusCode::kJsonTypeError;
    }

    auto& vote_process_status = params.vote_process_status;
    auto  n                   = json_data[k].size();
    auto  cc                  = static_cast<size_t>(VoteProcessStatus::count);
    if (n == 0 || n > cc) {
        return StatusCode::kLogicError;
    }
    vote_process_status.resize(n);
    for (int i = 0; i < n; ++i) {
        auto v = json_data[k][i].asUInt();
        if (v >= cc) {
            return StatusCode::kLogicError;
        }
        vote_process_status[i] = v;
    }
    return StatusCode::kSuccess;
}
common::StatusCode parse_send_vote_choices_req(const Json::Value&     json_data,
                                               SendVoteChoicesParams& params) {
    for (auto key : SendVoteChoicesReqKeys::keys) {
        if (!json_data.isMember(key)) {
            return StatusCode::kJsonKeyError;
        }
    }
    auto& json_vote_id = json_data[SendVoteChoicesReqKeys::vote_id_key];
    if (!json_vote_id.isUInt()) {
        return StatusCode::kJsonTypeError;
    }
    params.vote_id = json_vote_id.asUInt();

    auto& json_voter_id = json_data[SendVoteChoicesReqKeys::voter_id_key];
    if (!json_voter_id.isUInt()) {
        return StatusCode::kJsonTypeError;
    }
    params.voter_id = json_voter_id.asUInt();

    auto& json_voter_choices = json_data[SendVoteChoicesReqKeys::vote_choices_key];
    if (!json_voter_choices.isArray()) {
        return StatusCode::kJsonTypeError;
    }
    auto& voter_choices = params.voter_choices;
    voter_choices.reserve(json_voter_choices.size());
    for (auto& c : json_voter_choices) {
        voter_choices.push_back(c.asUInt());
    }
    return StatusCode::kSuccess;
}

}   // namespace server
}   // namespace tang