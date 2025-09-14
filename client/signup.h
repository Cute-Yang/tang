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

    void password_eye_checked(bool hide);

    void confirm_password_eye_checked(bool hide);
};
}   // namespace client
}   // namespace tang