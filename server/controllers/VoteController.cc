#include "VoteController.h"
#include "util.h"

// Add definition of your processing function here
using namespace tang::common;
using namespace tang::server::utils;

void VoteController::create_vote(const HttpRequestPtr& req,std::function<void(const HttpResponsePtr&)>&& callback){
    //parse it from the json!
    auto json_data = req->getJsonObject();
    if(json_data == nullptr){
        LOG_ERROR << "Fail to get json param,please give right!!!";
    }
}