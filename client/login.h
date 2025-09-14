#pragma once

#include "signin.h"
#include "signup.h"
#include <QStackedWidget>
#include "login_setting.h"

namespace tang {
namespace client {
class Login : public ElaWidget{
    Q_OBJECT
private:
    QStackedWidget* stack_container;
    SignIn* signin;
    SignUp* signup;
    ElaToolButton* setting_button;
    //setting page!
    LoginSetting* login_setting;

    void setup_ui();

    void hide_login_setting();

public:
    Login(QWidget* parent=nullptr);
    ~Login();

signals:
    void success_login();

private slots:
    void switch_to_signin();
    void switch_to_signup();

    void signin_clicked();

    // void signup_clicked();

    // void new_signup_clicked();

    void setting_button_clicked();
};
}
}