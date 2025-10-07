#pragma once
#include "common/status.h"
#include <json/json.h>

namespace tang {
namespace server {

struct ChunkVoteReqParams {
    uint32_t             voter_id;
    uint32_t             vote_num;
    uint32_t             vote_offset;
    std::vector<uint8_t> vote_status;
};

struct ChunkParticipateVoteReqParams : public ChunkVoteReqParams {
    std::vector<uint8_t> vote_process_status;
};

struct VoteNumReqParams {
    uint32_t             voter_id;
    std::vector<uint8_t> vote_status;
};

struct ParticipateVoteNumReqParams : public VoteNumReqParams {
    std::vector<uint8_t> vote_process_status;
};

struct SendVoteChoicesParams {
    std::vector<int> voter_choices;
    uint32_t         voter_id;
    uint32_t vote_id;
};


common::StatusCode parse_chunk_vote_data_req(const Json::Value&  json_data,
                                             ChunkVoteReqParams& params);
common::StatusCode parse_chunk_participate_vote_data_req(const Json::Value&             json_data,
                                                         ChunkParticipateVoteReqParams& params);
common::StatusCode parse_get_vote_num_req(const Json::Value& json_data, VoteNumReqParams& params);


common::StatusCode parse_get_participate_vote_num_req(const Json::Value&           json_data,
                                                      ParticipateVoteNumReqParams& params);



common::StatusCode parse_send_vote_choices_req(const Json::Value&     json_dataa,
                                               SendVoteChoicesParams& params);

}   // namespace server
}   // namespace tang