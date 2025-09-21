#pragma once
#include "common/status.h"
#include "drogon/WebSocketConnection.h"
#include <map>
#include <mutex>


namespace tang {
namespace server {
class ChatClientCollections {
public:
    // we need to save this conn,then we can send  the message to other clients!
    using websocket_conn_type = drogon::WebSocketConnectionPtr;
    // the key of websocket conn,make sure unique,here we use the vote_user_id as key
    using websocket_key_type = uint32_t;

private:
    std::mutex                                        mt;
    std::map<websocket_key_type, websocket_conn_type> connected_clients;
    ChatClientCollections() = default;

public:
    ~ChatClientCollections() = default;
    // delete copy funcs
    ChatClientCollections(const ChatClientCollections& rhs) = delete;
    ChatClientCollections(ChatClientCollections&& rhs)      = delete;

    //= operator!
    ChatClientCollections& operator=(const ChatClientCollections& rhs) = delete;
    ChatClientCollections& operator=(ChatClientCollections&& rhs)      = delete;

    common::StatusCode add_new_connection(websocket_key_type         conn_key,
                                          const websocket_conn_type& conn_ptr) noexcept;

    common::StatusCode remove_connection(websocket_key_type conn_key);
    common::StatusCode remove_connection(const websocket_conn_type& conn_ptr);

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