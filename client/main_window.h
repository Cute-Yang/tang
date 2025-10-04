#pragma once
#include "ElaWindow.h"
namespace tang {
namespace client {
class ClientMainWindow : public ElaWindow {
    Q_OBJECT
private:
    void init_page();

public:
    ClientMainWindow(QWidget* parent = nullptr);
    ~ClientMainWindow() {}

    void init_user_display_info();

    // private:
};
}   // namespace client
}   // namespace tang