#include "ElaMessageBar.h"
#include "client.h"
#include "client_global_config.h"
#include "client_singleton.h"
#include "util.h"
#include <QJsonObject>
#include <QTimer>


namespace tang {
namespace client {
Client::Client()
    : login(std::make_unique<Login>())
    , client_main_window(std::make_unique<ClientMainWindow>())
    , ws_chat_client(std::make_unique<QWebSocket>()) {
    login->show();
    this->init_connects();
}
Client::~Client() {}

void Client::show_message(const QString& message, bool error) {
    if (error) {
        ElaMessageBar::warning(ElaMessageBarType::TopLeft,
                               "client",
                               message,
                               ClientGlobalConfig::error_show_time,
                               client_main_window.get());
    } else {
        ElaMessageBar::success(ElaMessageBarType::TopLeft,
                               "client",
                               message,
                               ClientGlobalConfig::message_show_time,
                               client_main_window.get());
    }
}
void Client::init_connects() {
    auto login_callback = [this] {
        QTimer::singleShot(1200, [this]() {
            this->login->close();
            this->client_main_window->init_user_display_info();
            this->client_main_window->show();
            auto& current_user_info = ClientSingleton::get_cache_user_info_instance();
            auto  url = ClientSingleton::get_http_urls_instance().get_ws_chat_client_url(
                current_user_info.user_id, current_user_info.user_name);
            ws_chat_client->open(url);
            this->show_message(QString("欢迎你,Dear %1 (✿◠‿◠)!)").arg(current_user_info.user_name),
                               false);
        });
    };
    QObject::connect(
        login->signin, &SignIn::success_login, this->client_main_window.get(), login_callback);

    QObject::connect(
        ws_chat_client.get(), &QWebSocket::connected, client_main_window.get(), [this]() {
            auto& current_user_info = ClientSingleton::get_cache_user_info_instance();
            this->show_message(
                QString("Dear %1 加入了聊天室(✿◠‿◠)").arg(current_user_info.user_name), false);
        });

    // process data from other client!
    QObject::connect(
        ws_chat_client.get(),
        &QWebSocket::textFrameReceived,
        client_main_window.get(),
        [this](const QString& message) { this->process_message_from_other_client(message); });
}


void Client::process_message_from_other_client(const QString& message) {
    auto json_doc = get_json_document(message);
    if (!json_doc) {
        return;
    }
    auto json_data = json_doc->object();
    qDebug() << "connect from chat " << json_data;

    this->show_message(QString("您收到一条消息 %1").arg(qDebug().toString(json_data)), false);
}


}   // namespace client
}   // namespace tang