#include "WorkspaceController.h"
#include "../models/TestUsers.h"
#include "common/status.h"
#include "util.h"
#include <filesystem>
#include "drogon/utils/Utilities.h"


using namespace tang::common;
using namespace tang::server::utils;
using VoteUser = drogon_model::vote::TestUsers;

#define FWD(value) std::forward<decltype(value)>(value)

// the global database client name,equal to the config...
// maybe we should read it from the env or config??? not hard code it!
static const std::string db_client_name = "default";

#define make_response_and_return(status, callback)    \
    make_response_from_status_code(status, callback); \
    return;


void WorkspaceController::login(const HttpRequestPtr&                         req,
                                std::function<void(const HttpResponsePtr&)>&& callback) {
    std::string user_name = req->getParameter("username");
    std::string password  = req->getParameter("password");
    if (user_name.empty()) {
        make_response_and_return(StatusCode::UsernameIsEmpty, callback);
    } else if (password.empty()) {
        make_response_and_return(StatusCode::PasswordIsEmpty, callback);
    }
    auto db_client_ptr = app().getDbClient(db_client_name);
    if (db_client_ptr == nullptr) {
        // make_response_with_invalid_db(callback);
        make_response_and_return(StatusCode::InvalidDatabase, callback);
    }

    orm::Mapper<drogon_model::vote::TestUsers> user_mapper(db_client_ptr);
    if (user_mapper.count(
            orm::Criteria(VoteUser::Cols::_username, orm::CompareOperator::EQ, user_name)) == 0) {
        make_response_and_return(StatusCode::UsernameNotExist, callback);
    }

    auto result = user_mapper.findOne(
        orm::Criteria(VoteUser::Cols::_username, orm::CompareOperator::EQ, user_name));
    if (result.getValueOfPasswd() != password) {
        make_response_and_return(StatusCode::PasswordNotCorrect, callback);
    }

    // caputure by value,and not move,because the callback is a left value!
    // only capture by value
    auto c1 = [callback, result](const size_t n) {
        // the affect rows,if only query,will get 0,only chage will get > 0
        if (n != 0) {
            LOG_INFO << "the expected value is 1,but get " << n;
            make_response_and_return(StatusCode::LogicError, callback);
        }
        Json::Value ret = make_json_from_status_code(StatusCode::Success);
        ret["user_id"]  = result.getValueOfId();
        ret["email"]    = result.getValueOfEmail();
        auto resp       = HttpResponse::newHttpJsonResponse(ret);
        callback(resp);
    };
    auto c2 = [callback](const orm::DrogonDbException& ex) {
        LOG_ERROR << "sql error:" << ex.base().what();
        make_response_from_status_code(StatusCode::SqlRuntimeError, callback);
    };

    // set to activte!
    user_mapper.updateBy(
        {VoteUser::Cols::_is_active},
        c1,
        c2,
        orm::Criteria(VoteUser::Cols::_id, orm::CompareOperator::EQ, result.getValueOfId()),
        1);
}


void WorkspaceController::signup(const HttpRequestPtr&                         req,
                                 std::function<void(const HttpResponsePtr&)>&& callback) {
    Json::Value ret;
    // user_name
    std::string user_name = req->getParameter("username");
    if (user_name.empty()) {
        make_response_and_return(StatusCode::kUsernameIsEmpty, callback);
    }
    std::string password = req->getParameter("password");
    if (password.empty()) {
        make_response_and_return(StatusCode::kPasswordIsEmpty, callback);
    }
    std::string email         = req->getParameter("email");
    auto        db_client_ptr = app().getDbClient(db_client_name);
    if (db_client_ptr == nullptr) {
        make_response_and_return(StatusCode::kInvalidDatabase, callback);
    }

    orm::Mapper<VoteUser> user_mapper(db_client_ptr);

    if (user_mapper.count(
            orm::Criteria(VoteUser::Cols::_username, orm::CompareOperator::EQ, user_name)) != 0) {
        LOG_WARN << "user name :" << user_name << " is alreay exist!";
        make_response_and_return(StatusCode::kUsernameAlreayExit, callback);
    }

    //prepare insert data...
    VoteUser vote_user;
    vote_user.setUsername(user_name);
    vote_user.setPasswd(password);
    vote_user.setEmail(email);

    auto c1 = [callback](VoteUser vote_user) {
        LOG_INFO << "add new user with user_id = " << vote_user.getValueOfId()
                 << " user_name = " << vote_user.getValueOfUsername();
        make_response_and_return(StatusCode::kSuccess, callback);
    };

    auto c2 = [callback](const orm::DrogonDbException& ex) {
        LOG_ERROR << "db exception with " << ex.base().what();
        make_response_and_return(StatusCode::kSqlRuntimeError, callback);
    };
    user_mapper.insert(vote_user, c1, c2);
}


void WorkspaceController::get_file_infos(const HttpRequestPtr& req,std::function<void(const HttpResponsePtr&)>&& callback){
    std::string folder_path = req->getParameter("foler_path");
    if(folder_path.empty()){
        make_response_and_return(StatusCode::kFilePathIsEmpty,callback);
    }

    //convert from utf8 -> native!
    auto native_folder_path = std::filesystem::path(utils::toNativePath(folder_path)); 
    auto full_folder_path = get_workspace_root() / native_folder_path;
    std::error_code ec;
    if(!std::filesystem::exists(full_folder_path,ec)){
        LOG_ERROR << full_folder_path << " is not exist" << " " << ec.message();
        make_response_and_return(StatusCode::kFilePathNotExist,callback);
    }

    if(!std::filesystem::is_directory(full_folder_path,ec)){
        LOG_ERROR << "expected folder,but got file...";
        make_response_from_status_code(StatusCode::kFilePathIsNotFolder,callback);
    }
    
}


