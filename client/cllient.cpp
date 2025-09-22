#include "ElaMessageBar.h"
#include "client.h"
#include <QTimer>

namespace tang {
namespace client {
Client::Client()
    : login()
    , client_main_window() {
    login.show();
    QObject::connect(login.signin, &SignIn::success_login, &this->client_main_window, [this] {
        // this->login.close();
        // if using sleep,will block the main thread
        // never block the main block!
        QTimer::singleShot(2000, [this]() {
            this->login.close();
            this->client_main_window.show();
            ElaMessageBar::success(ElaMessageBarType::BottomRight,
                                   "login",
                                   "欢迎你,亲爱的lazydog!",
                                   2700,
                                   &this->client_main_window);
        });
    });
}

Client::~Client() {}
}   // namespace client
}   // namespace tang