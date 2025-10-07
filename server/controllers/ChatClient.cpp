#include "ChatClient.h"
#include "trantor/utils/Logger.h"

using namespace tang::common;
namespace tang {
namespace server {
StatusCode ChatClientCollections::add_new_connection(websocket_key_type    conn_key,
                                                     const ConnectionInfo& conn_info) noexcept {
    std::lock_guard<std::mutex> guard(this->mt);
    if (connected_clients.contains(conn_key)) {
        LOG_ERROR << "the conn key:" << conn_key << " is already exist!";
        return StatusCode::kWebSocketKeyIsAlreadyExist;
    }

    if (conn_info.conn_ptr == nullptr) {
        LOG_ERROR << "the conn ptr is nullptr...";
        return StatusCode::kWebSocketConnectionIsNullptr;
    }
    connected_clients.insert({conn_key, conn_info});
    return StatusCode::kSuccess;
}


std::pair<common::StatusCode, std::string> ChatClientCollections::remove_connection(
    websocket_key_type conn_key) {
    std::lock_guard<std::mutex> guard(this->mt);
    if (!connected_clients.contains(conn_key)) {
        return {StatusCode::kWebSocketKeyNotExsit, ""};
    }
    LOG_INFO << "Remove key:" << conn_key << " from websocket cache!";
    auto user_name = connected_clients.find(conn_key)->second.user_name;
    connected_clients.erase(conn_key);
    return {StatusCode::kSuccess, user_name};
}

// but very slow!
std::pair<common::StatusCode, std::string> ChatClientCollections::remove_connection(
    const drogon::WebSocketConnectionPtr& conn_ptr) {
    // foreach???
    for (auto& [k, conn_info] : connected_clients) {
        if (conn_info.conn_ptr == conn_ptr) {
            LOG_INFO << "remove key:" << k << " from websocket client cache!";
            connected_clients.erase(k);
            return {StatusCode::kSuccess, conn_info.user_name};

            break;
        }
    }
    return {StatusCode::kWebSocketKeyNotExsit, ""};
}

void ChatClientCollections::send_message(std::string_view message) const {
    for (auto& [_, conn_info] : connected_clients) {
        conn_info.conn_ptr->send(message);
    }
}

void ChatClientCollections::send_message(const Json::Value& message) const {
    for (auto& [_, conn_info] : connected_clients) {
        conn_info.conn_ptr->sendJson(message);
    }
}

ChatClientCollections& ChatClientCollections::get_instance() {
    static ChatClientCollections chat_instance;
    return chat_instance;
}



}   // namespace server
}   // namespace tang