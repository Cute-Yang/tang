#pragma once
#include "common/status.h"
#include "drogon/WebSocketConnection.h"
#include <map>
#include <mutex>


namespace tang {
namespace server {
class ChatClientCollections {
public:
    struct ConnectionInfo {
        drogon::WebSocketConnectionPtr conn_ptr;
        std::string                    user_name;
    };
    // we need to save this conn,then we can send  the message to other clients!
    // using websocket_conn_type = std::pair<drogon::WebSocketConnectionPtr,std::string>;/
    // the key of websocket conn,make sure unique,here we use the vote_user_id as key
    using websocket_key_type = uint32_t;

private:
    std::mutex                                   mt;
    std::map<websocket_key_type, ConnectionInfo> connected_clients;
    ChatClientCollections() = default;

public:
    ~ChatClientCollections() = default;
    // delete copy funcs
    ChatClientCollections(const ChatClientCollections& rhs) = delete;
    ChatClientCollections(ChatClientCollections&& rhs)      = delete;

    //= operator!
    ChatClientCollections& operator=(const ChatClientCollections& rhs) = delete;
    ChatClientCollections& operator=(ChatClientCollections&& rhs)      = delete;

    common::StatusCode add_new_connection(websocket_key_type    conn_key,
                                          const ConnectionInfo& conn_info) noexcept;

    std::pair<common::StatusCode,std::string> remove_connection(websocket_key_type conn_key);
     std::pair<common::StatusCode,std::string> remove_connection(const drogon::WebSocketConnectionPtr& conn_ptr);

    // here we only send text message,not consider the binary data!
    void send_message(std::string_view message) const;

    void send_message(const Json::Value& message) const;

    void send_message(const std::vector<websocket_key_type>& keys, std::string_view message) const;

    void send_message(const std::vector<websocket_key_type>& keys,
                      const Json::Value&                     message) const;

    void send_message(websocket_key_type key, std::string_view message);

    void send_message(websocket_key_type key, const Json::Value& message);

    // since c++11,static make sure thread safe!
    static ChatClientCollections& get_instance();
};


// singlton!



}   // namespace server
}   // namespace tang