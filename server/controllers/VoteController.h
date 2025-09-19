#pragma once

#include <drogon/HttpController.h>
#include <map>

class VoteData {
    std::string              vote_topic;
    std::vector<std::string> vote_topics;
    std::string              creator;
    std::string              create_time;
    std::vector<int>         voters;
    uint32_t                 vote_id;
    bool                     is_single_choice;
};

using namespace drogon;
class VoteController : public drogon::HttpController<VoteController> {
public:
    void create_vote(const HttpRequestPtr&                         req,
                     std::function<void(const HttpResponsePtr&)>&& callback);

    METHOD_LIST_BEGIN
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
    using vote_key = uint32_t;
    std::mutex                   vote_mt;
    std::map<vote_key, VoteData> vote_datas;
};
