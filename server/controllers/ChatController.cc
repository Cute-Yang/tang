#include "ChatController.h"
#include "ChatClient.h"
#include "common/chat_data.h"
#include "util.h"
#include <optional>

using namespace tang::common;
using namespace tang::server::utils;

std::optional<Json::Value> parse_json_from_str(const std::string& json_str) {
    Json::Value                             parsed_data;
    JSONCPP_STRING                          err;
    Json::CharReaderBuilder                 builder;
    const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
    if (!reader->parse(json_str.c_str(), json_str.c_str() + json_str.size(), &parsed_data, &err)) {
        // std::cout << "fail to parse json,error:" << err << std::endl;
        return {};
    }
    return {parsed_data};
}

Json::Value prepare_chat_message(const std::string& message) {
    Json::Value chat_message;
    chat_message[ChatJsonKeys::message_kind_key]  = static_cast<int>(ChatMessageKind::kNormalChat);
    chat_message[NormalChatJsonKeys::message_key] = message;
    chat_message[NormalChatJsonKeys::send_time_key] = get_current_time_str();
    chat_message[NormalChatJsonKeys::send_user_key] = "chatroom";
    return chat_message;
}

void ChatController::handleNewMessage(const WebSocketConnectionPtr& wsConnPtr,
                                      std::string&& message, const WebSocketMessageType& type) {

    auto _json_data = parse_json_from_str(message);
    if (!_json_data) {
        return;
    }
    auto& json_data = _json_data.value();

    auto message_kind =
        static_cast<ChatMessageKind>(json_data[ChatJsonKeys::message_kind_key].asInt());
    auto& chat_instance = tang::server::ChatClientCollections::get_instance();

    if (message_kind == ChatMessageKind::kNormalChat) {
        chat_instance.send_message(std::string_view(message.data(), message.size()));
    }
}

void ChatController::handleNewConnection(const HttpRequestPtr&         req,
                                         const WebSocketConnectionPtr& wsConnPtr) {
    auto     user_id_str = req->getParameter("user_id");
    auto     user_name   = req->getParameter("user_name");
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
    chat_instance.add_new_connection(user_id, {wsConnPtr, user_name});

    // notify to other person!
    Json::Value json_data = prepare_chat_message(user_name + "已经上线了(✿◠‿◠)");
    chat_instance.send_message(json_data);
}


void ChatController::handleConnectionClosed(const WebSocketConnectionPtr& wsConnPtr) {
    auto& chat_instance = tang::server::ChatClientCollections::get_instance();
    LOG_INFO << "Connection:" << wsConnPtr->peerAddr().toIpPort() << " disconnect!";


    auto [_, remove_user_name] = chat_instance.remove_connection(wsConnPtr);

    Json::Value json_data = prepare_chat_message(remove_user_name + "已经下线了(✿◠‿◠)");
    chat_instance.send_message(json_data);
}
