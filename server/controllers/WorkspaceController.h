#pragma once

#include <drogon/HttpController.h>
using namespace drogon;
class WorkspaceController : public drogon::HttpController<WorkspaceController> {
public:
    // get file infos
    void get_file_infos(const HttpRequestPtr&                         req,
                        std::function<void(const HttpResponsePtr&)>&& callback);

    // handle the workspace in another func!
    void get_workspace_infos(const HttpRequestPtr&                         req,
                             std::function<void(const HttpResponsePtr&)>&& callback);

    // file op
    void create_file(const HttpRequestPtr&                         req,
                     std::function<void(const HttpResponsePtr&)>&& callback);

    void create_dir(const HttpRequestPtr&                         req,
                    std::function<void(const HttpResponsePtr&)>&& callback);

    // include file and dir!
    void delete_file(const HttpRequestPtr&                         req,
                     std::function<void(const HttpResponsePtr&)>&& callback);

    // move contains the rename!!!
    void move_file(const HttpRequestPtr&                         req,
                   std::function<void(const HttpResponsePtr&)>&& callback);

    void upload_file(const HttpRequestPtr&                         req,
                     std::function<void(const HttpResponsePtr&)>&& callback);

    void download_file(const HttpRequestPtr&                         req,
                       std::function<void(const HttpResponsePtr&)>&& callback);

    void get_file_size(const HttpRequestPtr&                         req,
                       std::function<void(const HttpResponsePtr&)>&& callback);

    void get_workspace_names(const HttpRequestPtr&                         req,
                             std::function<void(const HttpResponsePtr&)>&& callback);

    void test_send_msg(const HttpRequestPtr&                         req,
                       std::function<void(const HttpResponsePtr&)>&& resp);

    // whether the file is exist!
    // all the file ops use json to pass the value,because the + will be encoded as ' '
    // return two fields,is_exsit:bool is_dir:bool
    void is_specify_file_exist(const HttpRequestPtr&                         req,
                               std::function<void(const HttpResponsePtr&)>&& resp);

    METHOD_LIST_BEGIN
    ADD_METHOD_TO(WorkspaceController::test_send_msg, "/api/v1/test_send_msg", Get, Post);
    ADD_METHOD_TO(WorkspaceController::get_file_infos, "/api/v1/get_file_infos", Post, Get);
    ADD_METHOD_TO(WorkspaceController::create_file, "/api/v1/create_file", Post);
    ADD_METHOD_TO(WorkspaceController::create_dir, "/api/v1/create_dir", Post);
    ADD_METHOD_TO(WorkspaceController::delete_file, "/api/v1/delete_file", Post);
    ADD_METHOD_TO(WorkspaceController::move_file, "/api/v1/move_file", Post);
    ADD_METHOD_TO(WorkspaceController::upload_file, "/api/v1/upload_file", Post);
    ADD_METHOD_TO(WorkspaceController::download_file, "/api/v1/download_file", Post);
    ADD_METHOD_TO(WorkspaceController::get_file_size, "/api/v1/get_file_size", Get, Post);
    ADD_METHOD_TO(WorkspaceController::get_workspace_names, "/api/v1/get_workspace_names", Get,
                  Post);
    ADD_METHOD_TO(WorkspaceController::is_specify_file_exist, "/api/v1/is_file_exist");

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
