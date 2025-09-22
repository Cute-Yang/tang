#pragma once

#include "signin.h"
#include "signup.h"
#include <QStackedWidget>
#include "login_setting.h"
#include <QNetworkReply>

namespace tang {
namespace client {
class Login : public ElaWidget{
    Q_OBJECT
private:
    QStackedWidget* stack_container;
    SignIn* signin;
    SignUp* signup;
    ElaToolButton* setting_button;
    LoginSetting* login_setting;
    void setup_ui();
    void hide_login_setting();

    void initialize_connects();

    void send_login_http_req();

    void process_login_response( QNetworkReply* reply);


public:
    Login(QWidget* parent=nullptr);
    ~Login();

signals:
    void success_login();

private slots:
    void switch_to_signin();
    void switch_to_signup();

    void on_signin_button_clicked();
    
    // void signup_clicked();

    // void new_signup_clicked();

    void on_setting_button_clicked();

};
}
}