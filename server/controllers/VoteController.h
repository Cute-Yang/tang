#pragma once

#include "VoteReqJson.h"
#include "common/status.h"
#include <drogon/HttpController.h>
#include <map>


using namespace drogon;


struct VoterInfo {
    std::vector<int> choices;
    // need use this to update datq
    uint64_t                        participate_id;
    tang::common::VoteProcessStatus process_status;
};

struct VoteResult {
    std::vector<int>     counts;
    std::vector<uint8_t> vote_items_status;
};

struct VoteItemInfo {
    std::string vote_item;
    // the return info of insert vote item!
    uint64_t vote_item_id;
};

struct VoteCacheInfo {
    std::vector<uint64_t> vote_item_ids;
    // k = voter_id cache the voter name and their choice!
    std::map<uint32_t, VoterInfo> voter_infos;
    uint32_t                      vote_creator_id;
    uint32_t                      n_finished{0};
    tang::common::VoteChoiceType  choice_type;
    // tang::common::VoteStatus      vote_status;
};

struct CreateVoteReqParams {
    std::string                  vote_topic;
    std::string                  vote_creator;
    uint32_t                     vote_creator_id;
    int64_t                      vote_create_time;
    std::vector<std::string>     vote_items;
    std::vector<uint32_t>        voter_ids;
    std::vector<std::string>     voter_names;
    tang::common::VoteChoiceType choice_type;
};

struct SendChoicesProcessResult {
    tang::common::StatusCode ret;
    uint64_t                 participate_id;
    bool                     finished;
};

// the database should add one column
//  means wheter the vote is valid!
class VoteController : public drogon::HttpController<VoteController> {
public:
    void create_vote(const HttpRequestPtr&                         req,
                     std::function<void(const HttpResponsePtr&)>&& callback);

    void remove_vote(const HttpRequestPtr&                         req,
                     std::function<void(const HttpResponsePtr&)>&& callback);

    void send_vote_choices(const HttpRequestPtr&                         req,
                           std::function<void(const HttpResponsePtr&)>&& callback);

    void get_online_voters(const HttpRequestPtr&                         req,
                           std::function<void(const HttpResponsePtr&)>&& callback);

    void get_vote_num(const HttpRequestPtr&                         req,
                      std::function<void(const HttpResponsePtr&)>&& callback);

    void get_chunk_vote_data(const HttpRequestPtr&                         req,
                             std::function<void(const HttpResponsePtr&)>&& callback);

    void get_participate_vote_num(const HttpRequestPtr&                         req,
                                  std::function<void(const HttpResponsePtr&)>&& callback);
    void get_chunk_participate_vote_data(const HttpRequestPtr&                         req,
                                         std::function<void(const HttpResponsePtr&)>&& callback);

    void get_finshied_vote_num(const HttpRequestPtr&                         req,
                               std::function<void(const HttpResponsePtr&)>&& callback);

    void get_chunk_finished_vote_data(const HttpRequestPtr&                         req,
                                      std::function<void(const HttpResponsePtr&)>&& callback);

    METHOD_LIST_BEGIN
    ADD_METHOD_TO(VoteController::create_vote, "/api/v1/create_vote", Post);
    ADD_METHOD_TO(VoteController::remove_vote, "/api/v1/remove_vote", Post);
    ADD_METHOD_TO(VoteController::send_vote_choices, "/api/v1/send_vote_choices", Post);
    ADD_METHOD_TO(VoteController::get_online_voters, "/api/v1/get_online_voters", Get, Post);
    ADD_METHOD_TO(VoteController::get_vote_num, "/api/v1/get_vote_num", Post);
    ADD_METHOD_TO(VoteController::get_chunk_vote_data, "/api/v1/get_chunk_vote_data", Post);
    ADD_METHOD_TO(VoteController::get_chunk_participate_vote_data,
                  "/api/v1/get_chunk_participate_vote_data", Post);
    ADD_METHOD_TO(VoteController::get_participate_vote_num, "/api/v1/get_participate_vote_num",
                  Post);
    ADD_METHOD_TO(VoteController::get_finshied_vote_num, "/api/v1/get_finished_vote_num", Get,
                  Post);
    ADD_METHOD_TO(VoteController::get_chunk_finished_vote_data,
                  "/api/v1/get_chunk_finished_vote_data", Post);
    // use METHOD_ADD to add your custom processing function here;
    // METHOD_ADD(VoteController::get, "/{2}/{1}", Get); // path is /VoteController/{arg2}/{arg1}
    // METHOD_ADD(VoteController::your_method_name, "/{1}/{2}/list", Get); // path is
    // /VoteController/{arg1}/{arg2}/list ADD_METHOD_TO(VoteController::your_method_name,
    // "/absolute/path/{1}/{2}/list", Get); // path is /absolute/path/{arg1}/{arg2}/list



    METHOD_LIST_END
    // your declaration of processing function maybe like this:
    // void get(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback,
    // int p1, std::string p2); void your_method_name(const HttpRequestPtr& req, std::function<void
    // (const HttpResponsePtr &)> &&callback, double p1, int p2) const;
private:
    std::mutex                        vote_mt;
    std::map<uint32_t, VoteCacheInfo> vote_datas;

    tang::common::StatusCode add_vote_data_to_cache(VoteCacheInfo&& vote_data,
                                                    uint32_t        vote_id) noexcept;
    tang::common::StatusCode remove_vote_data_from_cache(uint32_t vote_id) noexcept;

    bool compute_vote_result(const VoteCacheInfo& vote_data,
                             VoteResult&          vote_result) const noexcept;

    bool call_when_vote_finished(uint32_t vote_id);


    SendChoicesProcessResult process_voter_choices_impl(
        const tang::server::SendVoteChoicesParams& params);
};
