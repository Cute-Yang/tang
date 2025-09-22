#pragma once

#include "ElaWidget.h"
#include "signup_ui.h"
#include <QNetworkReply>

namespace tang {
namespace client {
class SignUp : public ElaWidget {
    Q_OBJECT
public:
    SignUpUi* ui;
    SignUp(QWidget* parent = nullptr);
    ~SignUp();

    void password_eye_checked(bool hide);
    void confirm_password_eye_checked(bool hide);

    void initialize_connects();

    void send_signup_http_req();

private slots:
    //the signup button clicked!
    void on_signup_button_clicked();
    //process the http response!
    void process_signup_response(QNetworkReply* reply);
};
}   // namespace client
}   // namespace tang