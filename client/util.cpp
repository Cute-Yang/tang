#include "util.h"
namespace tang {
namespace client {
void switch_password_eye_style(ElaLineEdit* line_edit, QAction* action, bool hide) {
    QString icon_file =
        hide ? ":/icons/images/hide_password.svg" : ":/icons/images/show_password.svg";
    ElaLineEdit::EchoMode echo_mode = hide ? ElaLineEdit::Password : ElaLineEdit::Normal;

    QIcon icon(icon_file);
    line_edit->setEchoMode(echo_mode);
    action->setIcon(icon);
}
}   // namespace client
}   // namespace tang