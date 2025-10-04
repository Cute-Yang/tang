#pragma once
#include "login.h"
#include "main_window.h"
#include <QWebSocket>
namespace tang {
namespace client {
class Client {
private:
    std::unique_ptr<Login>            login;
    std::unique_ptr<ClientMainWindow> client_main_window;
    std::unique_ptr<QWebSocket>       ws_chat_client;
    void                              init_connects();
    void                              show_message(const QString& message, bool error = true);

    void process_message_from_other_client(const QString& message);

public:
    Client();
    ~Client();
};
}   // namespace client
}   // namespace tang