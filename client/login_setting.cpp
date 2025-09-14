#include "login_setting.h"

namespace tang {
namespace client {
LoginSetting::LoginSetting(QWidget* parent)
    : ElaWidget(parent)
    , ui(new LoginSettingUi()) {
    ui->setup_ui(this);
    // this->setFixedSize(QSize(350, 520));
    this->setWindowTitle("setting");
    this->setWindowIcon(QIcon(":/icons/images/xiaoxiong.svg"));

    this->moveToCenter();
}
}   // namespace client
}   // namespace tang