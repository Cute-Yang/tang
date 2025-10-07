#pragma once
#include "ElaScrollPage.h"
#include "chat_room_ui.h"
#include "chat_view_model.h"

namespace tang {
namespace client {
class Client;
class ChatRoom : public ElaScrollPage {
    Q_OBJECT
private:
    ChatRoomUI*    ui;
    ChatViewModel* model;

    void prepare_chat_message();
    
    void init_connects();
signals:
    void send_chat_message(const QByteArray& message);

public:
    friend class Client;
    ChatRoom(QWidget* parent);
    void append_chat_message(const ChatMessage& message);

    ~ChatRoom();
};
}   // namespace client
}   // namespace tang