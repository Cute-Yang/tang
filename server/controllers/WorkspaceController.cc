#include "WorkspaceController.h"
#include "common/status.h"
#include "drogon/utils/Utilities.h"
#include "util.h"
#include <filesystem>
#include <fstream>
#include "ChatClient.h"

using namespace tang::common;
using namespace tang::server::utils;

#define handle_fs_error(ec, callback, ...)                                                    \
    if (ec) {                                                                                 \
        LOG_ERROR << "get filesystem error:" << ec.message() << " error_code:" << ec.value(); \
        make_response_and_return(StatusCode::kFileSystemRuntimeError, callback, __VA_ARGS__); \
    }

/*
if return false,means it is a invalid path...
*/
[[nodiscard]]
bool get_full_path(const std::string&                                 file_path,
                   const std::function<void(const HttpResponsePtr&)>& callback,
                   std::filesystem::path&                             full_path) {
    if (file_path.empty()) {
        make_response_and_return(StatusCode::kFilePathIsEmpty, callback, false);
    }

    if (file_path.starts_with("/") || file_path.starts_with('\\')) {
        // invalid!
        LOG_ERROR << "Path " << file_path << "  starts with root which is not allowed!";
        make_response_and_return(StatusCode::kUnexpectedAbsoluteFilePath, callback, false);
    }

    full_path =
        get_workspace_root() / std::filesystem::path(drogon::utils::toNativePath(file_path));
    return true;
}
/*
judge wheter the file exist,if exist,just return error!
*/
[[nodiscard("must handler the return value,and determin the next steps!")]]
bool raise_when_file_exit(const std::string&                                 file_path,
                          const std::function<void(const HttpResponsePtr&)>& callback,
                          std::filesystem::path&                             full_path) {
    if (!get_full_path(file_path, callback, full_path)) {
        return false;
    }
    std::error_code ec;
    auto            alreay_exit = std::filesystem::exists(file_path);
    handle_fs_error(ec, callback, false);
    ec.clear();
    if (alreay_exit) {
        LOG_ERROR << full_path << " is already exit!we can not create it!";
        make_response_and_return(StatusCode::kFileAlreadyExist, callback, false);
    }
    return true;
}



void WorkspaceController::get_file_infos(const HttpRequestPtr&                         req,
                                         std::function<void(const HttpResponsePtr&)>&& callback) {
    std::string           folder_path = req->getParameter("folder_path");
    std::filesystem::path full_folder_path;
    if (!get_full_path(folder_path, callback, full_folder_path)) {
        return;
    }
    std::error_code ec;
    bool            is_exit = std::filesystem::exists(full_folder_path, ec);
    handle_fs_error(ec, callback);
    ec.clear();
    if (!is_exit) {
        LOG_ERROR << full_folder_path << " is not exist" << " " << ec.message();
        make_response_and_return(StatusCode::kFilePathNotExist, callback);
    }

    bool is_dir = std::filesystem::is_directory(full_folder_path, ec);
    handle_fs_error(ec, callback);
    ec.clear();
    if (!is_dir) {
        LOG_ERROR << "expected folder,but got file...";
        make_response_from_status_code(StatusCode::kFilePathIsNotFolder, callback);
    }


    // utils::fromNativePath(full_folder_path.native());
    Json::Value ret        = make_json_from_status_code(StatusCode::kSuccess);
    Json::Value file_infos = Json::Value(Json::arrayValue);
    for (auto& entry : std::filesystem::directory_iterator(full_folder_path)) {
        Json::Value file_info;
        file_info["file_size"]       = entry.file_size();
        file_info["last_write_time"] = get_file_last_time_str(entry);
        // only get the name!
        file_info["file_name"] = utils::fromNativePath(entry.path().filename().native());
        // determin the file type
        if (entry.is_directory()) {
            file_info["file_type"] = static_cast<int>(FileKind::kFolder);
        } else if (entry.is_regular_file()) {
            file_info["file_type"] = static_cast<int>(get_file_kind(entry.path()));
        } else {
            // other,not consider!
            file_info["file_type"] = static_cast<int>(FileKind::kOthers);
            LOG_WARN << "unknown file...";
        }
        file_infos.append(file_info);
    }
    ret["file_infos"] = file_infos;
    auto resp         = HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}


void WorkspaceController::delete_file(const HttpRequestPtr&                         req,
                                      std::function<void(const HttpResponsePtr&)>&& callback) {
    std::string           file_path = req->getParameter("file_path");
    std::filesystem::path full_path;
    if (!get_full_path(file_path, callback, full_path)) {
        return;
    }

    std::error_code ec;
    auto            is_exist = std::filesystem::exists(full_path, ec);
    handle_fs_error(ec, callback);
    if (!is_exist) {
        LOG_ERROR << "Can not delete none exist path: " << full_path;
        make_response_and_return(StatusCode::kFilePathNotExist, callback);
    }
    ec.clear();

    auto is_dir = std::filesystem::is_directory(full_path, ec);
    handle_fs_error(ec, callback);
    ec.clear();
    auto ok = false;
    if (!is_dir) {
        ok = std::filesystem::remove(full_path, ec);
    } else {
        std::filesystem::remove_all(full_path, ec);
        // if remove dir ok,we can not find it!
        //  if any failur occur,the dir also exist! ^_^
        ok = !std::filesystem::exists(full_path, ec);
    }
    handle_fs_error(ec, callback);

    if (!ok) {
        LOG_ERROR << "Fail to remove path:" << full_path;
        make_response_and_return(StatusCode::kFailToRemoveFile, callback);
    }

    make_response_from_status_code(StatusCode::kSuccess, callback);
}


// only support create file at directory,not support give a path!
void WorkspaceController::create_file(const HttpRequestPtr&                         req,
                                      std::function<void(const HttpResponsePtr&)>&& callback) {
    std::string           file_path = req->getParameter("file_path");
    std::filesystem::path full_path;
    if (!raise_when_file_exit(file_path, callback, full_path)) {
        return;
    }
    // make native!
    std::ofstream f(full_path.native());
    if (!f.good()) {
        LOG_ERROR << "fail to create file " << full_path;
        make_response_and_return(StatusCode::kFailToCreateFile, callback);
    }
    f.close();
    LOG_INFO << "create file " << full_path << " sucessfully!";
    make_response_and_return(StatusCode::kSuccess, callback);
}


void WorkspaceController::create_dir(const HttpRequestPtr&                         req,
                                     std::function<void(const HttpResponsePtr&)>&& callback) {
    std::string           file_path = req->getParameter("file_path");
    std::filesystem::path full_path;
    if (!raise_when_file_exit(file_path, callback, full_path)) {
        return;
    }
    std::error_code ec;
    auto            ok = std::filesystem::create_directory(full_path);
    handle_fs_error(ec, callback);

    if (!ok) {
        LOG_ERROR << "Fail to create dir " << full_path;
        make_response_and_return(StatusCode::kFailToCreateDir, callback);
    }

    make_response_and_return(StatusCode::kSuccess, callback);
}

void WorkspaceController::move_file(const HttpRequestPtr&                         req,
                                    std::function<void(const HttpResponsePtr&)>&& callback) {
    std::string           source_file_path = req->getParameter("source_file_path");
    std::string           target_file_path = req->getParameter("target_file_path");
    std::filesystem::path source_full_path;
    std::filesystem::path target_full_path;
    if (!get_full_path(source_file_path, callback, source_full_path) ||
        !get_full_path(target_file_path, callback, target_full_path)) {
        return;
    }

    std::error_code ec;
    std::filesystem::rename(source_full_path, target_full_path, ec);
    handle_fs_error(ec, callback);

    LOG_INFO << "Move file from " << source_full_path << " -> " << target_full_path;
    make_response_and_return(StatusCode::kSuccess, callback);
}


void WorkspaceController::upload_file(const HttpRequestPtr&                         req,
                                      std::function<void(const HttpResponsePtr&)>&& callback) {
    MultiPartParser part_parser;
    if (part_parser.parse(req) != 0) {
        LOG_ERROR << "Fail to parse multi part params!";
        make_response_and_return(StatusCode::kUploadFilesIsEmpty, callback);
    }
    auto save_dir = part_parser.getParameters().at("save_dir");
    LOG_INFO << save_dir;
    std::filesystem::path full_save_dir;
    if (!get_full_path(save_dir, callback, full_save_dir)) {
        return;
    }

    std::error_code ec;
    bool            is_save_dir_exsit = std::filesystem::exists(full_save_dir, ec);
    handle_fs_error(ec, callback);

    if (!is_save_dir_exsit) {
        LOG_ERROR << full_save_dir << " is not exist?"
                  << " we expected utf8 encode file path,maybe not??";
        make_response_and_return(StatusCode::kFilePathNotExist, callback);
    }

    auto save_files = part_parser.getFiles();
    if (save_files.size() == 0) {
        LOG_ERROR << "Unexpected file size == 0";
        make_response_and_return(StatusCode::kUploadFilesIsEmpty, callback);
    }

    for (size_t i = 0; i < save_files.size(); ++i) {
        // make sure utf8
        auto filename = save_files[i].getFileName();
        auto full_file_path =
            full_save_dir / std::filesystem::path(drogon::utils::toNativePath(filename));
        // write the data!
        // convert it -> string with utf-8 encode!
        auto save_file_u8 = drogon::utils::fromNativePath(full_file_path.native());
        save_files[i].saveAs(save_file_u8);
    }
    make_response_and_return(StatusCode::kSuccess, callback);
}


void WorkspaceController::download_file(const HttpRequestPtr&                         req,
                                        std::function<void(const HttpResponsePtr&)>&& callback) {
    // only need a file path!
    // make sure encode with utf8
    std::string file_path = req->getParameter("file_path");
    // convert it -> fs
    std::filesystem::path full_file_path;

    if (!get_full_path(file_path, callback, full_file_path)) {
        LOG_ERROR << "File path :" << file_path << " is not exist,maybe not utf8??";
        make_response_and_return(StatusCode::kFilePathNotExist, callback);
    }

    // just return a file response!
    auto file_path_u8 = drogon::utils::fromNativePath(full_file_path.native());
    // filename
    auto file_name_u8 = drogon::utils::fromNativePath(full_file_path.filename());
    auto resp         = HttpResponse::newFileResponse(file_path_u8, file_name_u8);
    callback(resp);
}


// for download,if large file,we will download it by chunk,and save it to the temprary dir!
void WorkspaceController::get_file_size(const HttpRequestPtr&                         req,
                                        std::function<void(const HttpResponsePtr&)>&& callback) {
    std::string           file_path = req->getParameter("file_path");
    std::filesystem::path full_file_path;

    if (!raise_when_file_exit(file_path, callback, full_file_path)) {
        return;
    }
    std::error_code ec;
    auto            file_size = std::filesystem::file_size(full_file_path, ec);
    handle_fs_error(ec, callback);

    auto ret         = make_json_from_status_code(StatusCode::kSuccess);
    ret["file_size"] = file_size;

    auto resp = HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}


void WorkspaceController::test_send_msg(const HttpRequestPtr& req,std::function<void(const HttpResponsePtr&)>&& callback){
    auto& chat_instance = tang::server::ChatClientCollections::get_instance();
    std::string_view msg = "The brownfox jumps over the lazydog!";
    //here the c_string literal can convert to json value implicit!
    chat_instance.send_message(msg);

    make_response_only(StatusCode::kSuccess,callback);
}