#pragma once
#include "ElaWindow.h"
#include "chat_room.h"

namespace tang {
namespace client {
class Client;
class ClientMainWindow : public ElaWindow {
    Q_OBJECT
private:
    ChatRoom* chat_room;
    void      init_page();

public:
    friend class Client;
    ClientMainWindow(QWidget* parent = nullptr);
    ~ClientMainWindow() {}

    void init_user_display_info();

    // private:
};
}   // namespace client
}   // namespace tang