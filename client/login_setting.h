#pragma once
#include "login_setting_ui.h"
#include "ElaWidget.h"
namespace tang {
namespace client {
class LoginSetting:public ElaWidget{
Q_OBJECT
public:
    LoginSettingUi* ui;
    LoginSetting(QWidget* parent = nullptr);
};
}
}