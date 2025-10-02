#include "ChatController.h"
#include "ChatClient.h"

void ChatController::handleNewMessage(const WebSocketConnectionPtr& wsConnPtr,
                                      std::string&& message, const WebSocketMessageType& type) {
    // write your application logic here
    // LOG_INFO << "New msg:" << message << " from:" << wsConnPtr->peerAddr().toIpPort();
    // wsConnPtr->send("The brownfox jumps over the lazydog!");
    // Json::Value json_data;
    // json_data["s1"] = 32;
    // json_data["s2"] = "lazydog";
    // json_data["s3"].append(32);
    // json_data["s3"].append(48);
    // wsConnPtr->sendJson(json_data);
}

void ChatController::handleNewConnection(const HttpRequestPtr&         req,
                                         const WebSocketConnectionPtr& wsConnPtr) {
    auto user_id_str = req->getParameter("user_id");
    auto user_name   = req->getParameter("user_name");

    uint32_t user_id;
    // parse the user id from url?
    auto result =
        std::from_chars(user_id_str.data(), user_id_str.data() + user_id_str.size(), user_id);
    if (result.ec != std::errc()) {
        // the user id is not correct!!
        LOG_ERROR << "Invalid user id: " << user_id_str << "which can not convert to uint32_t...";
        wsConnPtr->forceClose();
        return;
    }

    LOG_INFO << "New connection:" << wsConnPtr->peerAddr().toIpPort() << "user_name:" << user_name
             << " user_id:" << user_id_str << " join our chatroom!";
    auto& chat_instance = tang::server::ChatClientCollections::get_instance();
    chat_instance.add_new_connection(user_id, wsConnPtr);
}


void ChatController::handleConnectionClosed(const WebSocketConnectionPtr& wsConnPtr) {
    auto& chat_instance = tang::server::ChatClientCollections::get_instance();
    chat_instance.remove_connection(wsConnPtr);
    LOG_INFO << "Connection:" << wsConnPtr->peerAddr().toIpPort() << " disconnect!";
}
