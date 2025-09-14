#include "signup.h"
#include "util.h"

namespace tang {
namespace client {

SignUp::SignUp(QWidget* parent)
    : ElaWidget(parent)
    , ui(new SignUpUi()) {
    ui->setup_ui(this);
    // this->moveToCenter();
    // this->setFixedSize(QSize(350,520));
    this->setWindowTitle("signup~");
    // this->setWindowOpacity(0.98);
    this->setWindowIcon(QIcon(":/icons/images/changjinglu.svg"));


    // connects
    connect(ui->password_eye_action, &QAction::toggled, this, &SignUp::password_eye_checked);

    connect(ui->confirm_password_eye_action,
            &QAction::toggled,
            this,
            &SignUp::confirm_password_eye_checked);
}

void SignUp::password_eye_checked(bool hide) {
    switch_password_eye_style(ui->password_line_edit, ui->password_eye_action, hide);
}

void SignUp::confirm_password_eye_checked(bool hide) {
    switch_password_eye_style(
        ui->confirm_password_line_edit, ui->confirm_password_eye_action, hide);
}
}   // namespace client
}   // namespace tang