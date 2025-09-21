#include "ChatClient.h"
#include "trantor/utils/Logger.h"

using namespace tang::common;
namespace tang {
namespace server {
StatusCode ChatClientCollections::add_new_connection(websocket_key_type         conn_key,
                                                     const websocket_conn_type& conn_ptr) noexcept {
    std::lock_guard<std::mutex> guard(this->mt);
    if (connected_clients.contains(conn_key)) {
        LOG_ERROR << "the conn key:" << conn_key << " is already exist!";
        return StatusCode::kWebSocketKeyIsAlreadyExist;
    }

    if (conn_ptr == nullptr) {
        LOG_ERROR << "the conn ptr is nullptr...";
        return StatusCode::kWebSocketConnectionIsNullptr;
    }
    connected_clients.insert({conn_key, conn_ptr});
    return StatusCode::kSuccess;
}


StatusCode ChatClientCollections::remove_connection(websocket_key_type conn_key) {
    std::lock_guard<std::mutex> guard(this->mt);
    if (!connected_clients.contains(conn_key)) {
        return StatusCode::kWebSocketKeyNotExsit;
    }
    LOG_INFO << "Remove key:" << conn_key << " from websocket cache!";
    connected_clients.erase(conn_key);
    return StatusCode::kSuccess;
}

// but very slow!
StatusCode ChatClientCollections::remove_connection(const websocket_conn_type& conn_ptr) {
    // foreach???
    for (auto& [k, v] : connected_clients) {
        if (v == conn_ptr) {
            LOG_INFO << "remove key:" << k << " from websocket client cache!";
            connected_clients.erase(k);
            break;
        }
    }
    return StatusCode::kSuccess;
}

void ChatClientCollections::send_message(std::string_view message) const {
    for (auto& [_, conn_ptr] : connected_clients) {
        conn_ptr->send(message);
    }
}

void ChatClientCollections::send_message(const Json::Value& message) const {
    for (auto& [_, conn_ptr] : connected_clients) {
        conn_ptr->sendJson(message);
    }
}

ChatClientCollections& ChatClientCollections::get_instance() {
    static ChatClientCollections chat_instance;
    return chat_instance;
}



}   // namespace server
}   // namespace tang