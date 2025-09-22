#pragma once

#include "login_setting.h"
#include "signin.h"
#include "signup.h"
#include <QStackedWidget>


namespace tang {
namespace client {
class Client;
class Login : public ElaWidget {
    Q_OBJECT
    friend class Client;

private:
    QStackedWidget* stack_container;
    SignIn*         signin;
    SignUp*         signup;
    ElaToolButton*  setting_button;
    LoginSetting*   login_setting;
    void            setup_ui();
    void            hide_login_setting();
    void            initialize_connects();

public:
    Login(QWidget* parent = nullptr);
    ~Login();

private slots:
    void transfer_to_signin();
    void transfer_to_signup();
    void on_setting_button_clicked();
};
}   // namespace client
}   // namespace tang