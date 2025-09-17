#include "WorkspaceController.h"
#include "../models/TestUsers.h"
#include "common/status.h"


using namespace tang::common;
using VoteUser = drogon_model::vote::TestUsers;

static const std::string db_client_name = "default";

#define make_response_and_return(ret)                   \
    auto resp = HttpResponse::newHttpJsonResponse(ret); \
    callback(resp);                                     \
    return;

// Add definition of your processing function here

void WorkspaceController::login(const HttpRequestPtr&                         req,
                                std::function<void(const HttpResponsePtr&)>&& callback) {

    std::string user_name = req->getParameter("user_name");
    std::string password  = req->getParameter("password");
    Json::Value ret;
    if (user_name.empty()) {
        ret["status"]  = static_cast<int>(StatusCode::UsernameIsEmpty);
        ret["message"] = "用户名为空!";
        make_response_and_return(ret);
    } else if (password.empty()) {
        ret["status"]  = static_cast<int>(StatusCode::PasswordIsEmpty);
        ret["message"] = "用户密码为空!";
        make_response_and_return(ret);
    }

    auto db_client_ptr = app().getDbClient(db_client_name);
    if (db_client_ptr == nullptr) {
        LOG_WARN << "the database is disconnected,get nullptr!";
        ret["status"]  = static_cast<int>(StatusCode::InvalidDatabase);
        ret["message"] = "数据库断开连接!";
        make_response_and_return(ret);
    }

    orm::Mapper<drogon_model::vote::TestUsers> user_mapper(db_client_ptr);
    if (user_mapper.count(
            orm::Criteria(VoteUser::Cols::_username, orm::CompareOperator::EQ, user_name)) == 0) {
        // the user is not exist!
        ret["status"] = static_cast<int>(StatusCode::UsernameNotExist);
        make_response_and_return(ret);
    }

    auto result = user_mapper.findOne(
        orm::Criteria(VoteUser::Cols::_username, orm::CompareOperator::EQ, user_name));
    if (result.getValueOfPasswd() != password) {
        ret["status"] = static_cast<int>(StatusCode::PasswordNotCorrect);
        make_response_and_return(ret);
    }

    // return user_name user_id,
    // and set it to activate!
    auto c1 = [callback](const size_t n) {
        Json::Value ret;
        if (n != 1) {
            // never
            ret["status"]  = static_cast<int>(StatusCode::LogicError);
            ret["message"] = "非预期的逻辑错误!";
        } else {
            ret["status"]  = static_cast<int>(StatusCode::Success);
            ret["message"] = "登陆成功!";
        }
        make_response_and_return(ret);
    };
    auto c2 = [callback](const orm::DrogonDbException& ex) {
        Json::Value ret;
        ret["status"]  = static_cast<int>(StatusCode::SqlRuntimeError);
        ret["message"] = ex.base().what();
        make_response_and_return(ret);
    };
    user_mapper.updateBy(
        {VoteUser::Cols::_is_active},
        c1,
        c2,
        orm::Criteria(VoteUser::Cols::_id, orm::CompareOperator::EQ, result.getValueOfId()),
        1);
}