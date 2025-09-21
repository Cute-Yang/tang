#pragma once

#include <drogon/WebSocketController.h>

using namespace drogon;

class ChatController : public drogon::WebSocketController<ChatController> {
public:
    void handleNewMessage(const WebSocketConnectionPtr&, std::string&&,
                          const WebSocketMessageType&) override;
    void handleNewConnection(const HttpRequestPtr&, const WebSocketConnectionPtr&) override;
    void handleConnectionClosed(const WebSocketConnectionPtr&) override;
    WS_PATH_LIST_BEGIN
    // list path definitions here;
    // WS_PATH_ADD("/path", "filter1", "filter2", ...);
    WS_PATH_ADD("/api/v1/chat");
    WS_PATH_LIST_END
};
