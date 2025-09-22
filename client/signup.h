#pragma once

#include "ElaWidget.h"
#include "signup_ui.h"

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

    void send_sign_up_http_req();
private slots:
    //the signup button clicked!
    void on_sign_up_button_clicked();
    //back 
    void on_back_button_clicked();
};
}   // namespace client
}   // namespace tang