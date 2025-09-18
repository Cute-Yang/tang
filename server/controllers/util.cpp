#include "util.h"

namespace tang {
namespace server {
namespace utils {
Json::Value make_json_from_status_code(common::StatusCode status) {
    Json::Value ret;
    ret["status"]  = static_cast<int>(status);
    ret["message"] = tang::common::get_status_str(status).data();
    return ret;
}

void make_response_from_status_code(
    common::StatusCode status, const callback_func& callback) {
    auto ret  = make_json_from_status_code(status);
    auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

std::filesystem::path& get_workspace_root(){
    static std::filesystem::path workspace_root ="";
    return workspace_root; 
}
}   // namespace utils
}   // namespace server
}   // namespace tang