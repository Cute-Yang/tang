#pragma once
#include "common/status.h"
#include "drogon/HttpResponse.h"
#include <filesystem>


namespace tang {
namespace server {
namespace utils {

#define FWD(value) std::forward<decltype(value)>(value)

// the global database client name,equal to the config...
// maybe we should read it from the env or config??? not hard code it!


#define make_response_only(status, callback) make_response_from_status_code(status, callback);

#define make_response_and_return(status, callback, ...) \
    make_response_from_status_code(status, callback);   \
    return __VA_ARGS__;

// implicit convert,fuck cxx!!!
using callback_func = std::function<void(const drogon::HttpResponsePtr&)>;
// using callback_func_rr = callback_func&&;

Json::Value make_json_from_status_code(common::StatusCode status);


void make_response_from_status_code(common::StatusCode status, const callback_func& callback);

// this should config from the  config,call this func -> main func!
std::filesystem::path& get_workspace_root();
std::string&           get_db_client_name();

common::FileKind get_file_kind(const std::filesystem::path& fp);

void to_lower(char* s, size_t len);

std::string get_file_last_time_str(const std::filesystem::directory_entry& p);


[[nodiscard]]
common::StatusCode get_full_path(const std::string& file_path, std::filesystem::path& full_path);



template<class T>
[[nodiscard]]
bool convert_str(std::string_view data, T& ret) {
    auto r = std::from_chars(data.data(), data.data() + data.size(), ret);
    return r.ec == std::errc();
}
}   // namespace utils
}   // namespace server
}   // namespace tang
