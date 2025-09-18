#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

class WorkspaceController : public drogon::HttpController<WorkspaceController> {
public:
    // login!
    void login(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback);

    //sigup
    void signup(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback);

    //get file infos

    void get_file_infos(const HttpRequestPtr& req,std::function<void(const HttpResponsePtr&)>&& callback);

    METHOD_LIST_BEGIN
    ADD_METHOD_TO(WorkspaceController::login, "/api/v1/login", Get,Post);
    ADD_METHOD_TO(WorkspaceController::signup, "api/v1/signup", Get,Post);
    // use METHOD_ADD to add your custom processing function here;
    // METHOD_ADD(WorkspaceController::get, "/{2}/{1}", Get); // path is
    // /WorkspaceController/{arg2}/{arg1} METHOD_ADD(WorkspaceController::your_method_name,
    // "/{1}/{2}/list", Get); // path is /WorkspaceController/{arg1}/{arg2}/list
    // ADD_METHOD_TO(WorkspaceController::your_method_name, "/absolute/path/{1}/{2}/list", Get); //
    // path is /absolute/path/{arg1}/{arg2}/list

    METHOD_LIST_END
    // your declaration of processing function maybe like this:
    // void get(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback,
    // int p1, std::string p2); void your_method_name(const HttpRequestPtr& req, std::function<void
    // (const HttpResponsePtr &)> &&callback, double p1, int p2) const;
};
