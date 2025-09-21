#pragma once

#include "common/status.h"
#include <drogon/HttpController.h>
#include <map>

using namespace drogon;

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

    METHOD_LIST_BEGIN
    ADD_METHOD_TO(VoteController::create_vote, "/api/v1/create_vote", Post);
    ADD_METHOD_TO(VoteController::remove_vote, "/api/v1/remove_vote", Post);
    ADD_METHOD_TO(VoteController::send_vote_choices, "/api/v1/send_vote_choices", Post);
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
    using vote_id_type      = uint32_t;
    using voter_id_type     = uint32_t;
    using voter_choice_type = std::vector<int>;

    struct VoteResult {
        std::vector<int> counts;
        std::vector<int> ret;
    };

    struct VoteCacheData {
        // topic
        std::string vote_topic;
        // items
        std::vector<std::string> vote_items;
        // creator
        std::string vote_creator;
        // create time! give the timestap!!!
        int64_t vote_create_time;
        // std::vector<vote_user_id_type> voters;
        std::map<voter_id_type, voter_choice_type> voter_choices;
        // alreay
        int           n_finished_voter;
        voter_id_type vote_creator_id;
        vote_id_type  vote_id;

        tang::common::VoteChoiceType choice_type;
        tang::common::VoteStatus     vote_status;
    };

    std::mutex                            vote_mt;
    std::map<vote_id_type, VoteCacheData> vote_datas;

    tang::common::StatusCode add_vote_data_to_cache(VoteCacheData&& vote_data) noexcept;
    tang::common::StatusCode remove_vote_data_from_cache(vote_id_type) noexcept;

    VoteResult compute_vote_result(const VoteCacheData& vote_data) const noexcept;

    bool vote_finished_work(vote_id_type vote_id);
};
