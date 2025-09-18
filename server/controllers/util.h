#pragma once
#include "common/status.h"
#include "drogon/HttpResponse.h"
#include <filesystem>


namespace tang {
namespace server {
namespace utils {
// implicit convert,fuck cxx!!!
using callback_func = std::function<void(const drogon::HttpResponsePtr&)>;
// using callback_func_rr = callback_func&&;

Json::Value make_json_from_status_code(common::StatusCode status);


void make_response_from_status_code(common::StatusCode status, const callback_func& callback);


std::filesystem::path& get_workspace_root();

}   // namespace utils
}   // namespace server
}   // namespace tang