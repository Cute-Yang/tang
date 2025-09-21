#include "SignController.h"
#include "../models/TestVoteUser.h"
#include "common/status.h"
#include "util.h"
using namespace tang::common;
using namespace tang::server::utils;
using VoteUser = drogon_model::vote::TestVoteUser;
// Add definition of your processing function here


void SignController::login(const HttpRequestPtr&                         req,
                           std::function<void(const HttpResponsePtr&)>&& callback) {
    std::string user_name = req->getParameter("username");
    std::string password  = req->getParameter("password");
    if (user_name.empty()) {
        make_response_and_return(StatusCode::kUsernameIsEmpty, callback);
    } else if (password.empty()) {
        make_response_and_return(StatusCode::kPasswordIsEmpty, callback);
    }
    auto db_client_ptr = app().getDbClient(get_db_client_name());
    if (db_client_ptr == nullptr) {
        // make_response_with_invalid_db(callback);
        make_response_and_return(StatusCode::kInvalidDatabase, callback);
    }

    orm::Mapper<VoteUser> user_mapper(db_client_ptr);
    if (user_mapper.count(
            orm::Criteria(VoteUser::Cols::_user_name, orm::CompareOperator::EQ, user_name)) == 0) {
        make_response_and_return(StatusCode::kUsernameNotExist, callback);
    }

    auto result = user_mapper.findOne(
        orm::Criteria(VoteUser::Cols::_user_name, orm::CompareOperator::EQ, user_name));
    if (result.getValueOfPasswd() != password) {
        make_response_and_return(StatusCode::kPasswordNotCorrect, callback);
    }

    // caputure by value,and not move,because the callback is a left value!
    // only capture by value
    auto c1 = [callback, result](const size_t n) {
        // the affect rows,if only query,will get 0,only chage will get > 0
        if (n != 0) {
            LOG_INFO << "the expected value is 1,but get " << n;
            make_response_and_return(StatusCode::kLogicError, callback);
        }
        Json::Value ret = make_json_from_status_code(StatusCode::kSuccess);
        ret["user_id"]  = result.getValueOfId();
        ret["email"]    = result.getValueOfEmail();
        auto resp       = HttpResponse::newHttpJsonResponse(ret);
        callback(resp);
    };
    auto c2 = [callback](const orm::DrogonDbException& ex) {
        LOG_ERROR << "sql error:" << ex.base().what();
        make_response_from_status_code(StatusCode::kSqlRuntimeError, callback);
    };

    // set to activte!
    user_mapper.updateBy(
        {VoteUser::Cols::_user_status},
        c1,
        c2,
        orm::Criteria(VoteUser::Cols::_id, orm::CompareOperator::EQ, result.getValueOfId()),
        VoteUserStatus::kActive);
}


void SignController::signup(const HttpRequestPtr&                         req,
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
    auto        db_client_ptr = app().getDbClient(get_db_client_name());
    if (db_client_ptr == nullptr) {
        make_response_and_return(StatusCode::kInvalidDatabase, callback);
    }

    orm::Mapper<VoteUser> user_mapper(db_client_ptr);

    if (user_mapper.count(
            orm::Criteria(VoteUser::Cols::_user_name, orm::CompareOperator::EQ, user_name)) != 0) {
        LOG_WARN << "user name :" << user_name << " is alreay exist!";
        make_response_and_return(StatusCode::kUsernameAlreadyExist, callback);
    }

    // prepare insert data...
    VoteUser vote_user;
    vote_user.setUserName(user_name);
    vote_user.setPasswd(password);
    vote_user.setEmail(email);

    auto c1 = [callback](VoteUser vote_user) {
        LOG_INFO << "add new user with user_id = " << vote_user.getValueOfId()
                 << " user_name = " << vote_user.getValueOfUserName();
        make_response_and_return(StatusCode::kSuccess, callback);
    };

    auto c2 = [callback](const orm::DrogonDbException& ex) {
        LOG_ERROR << "db exception with " << ex.base().what();
        make_response_and_return(StatusCode::kSqlRuntimeError, callback);
    };
    user_mapper.insert(vote_user, c1, c2);
}
