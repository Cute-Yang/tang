#include "login.h"
#include "ElaMessageBar.h"
#include "ElaToolBar.h"

namespace tang {
namespace client {
Login::Login(QWidget* parent)
    : ElaWidget(parent) {
    signin        = new SignIn(this);
    signup        = new SignUp(this);
    login_setting = new LoginSetting();
    login_setting->hide();
    setup_ui();
    this->moveToCenter();
    this->setFixedSize(QSize(350, 520));
    this->setWindowTitle("login");
    this->setWindowIcon(QIcon(":/icons/images/changjinglu.svg"));
    // connect!
    connect(signin->ui->sign_up_button, &ElaToolButton::clicked, this, &Login::switch_to_signup);
    connect(signup->ui->back_button, &ElaToolButton::clicked, this, &Login::switch_to_signin);

    connect(signin->ui->sign_in_button, &ElaToolButton::clicked, this, &Login::signin_clicked);


    connect(setting_button, &ElaToolButton::clicked, this, &Login::setting_button_clicked);
}

Login::~Login(){
    delete login_setting;
}

void Login::hide_login_setting() {
    if (!login_setting->isHidden()) {
        login_setting->hide();
    }
}

void Login::setup_ui() {
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins({0, 0, 0, 0});

    QWidget* tool_bar_container = new QWidget(this);
    tool_bar_container->setFixedHeight(36);
    tool_bar_container->setObjectName("tool_bar_container");
    QVBoxLayout* tool_bar_layout = new QVBoxLayout(tool_bar_container);
    QString      tool_bar_style  = R"style(
        QWidget#tool_bar_container{
            background-color:#ADD8E6;
            }
    )style";
    tool_bar_container->setStyleSheet(tool_bar_style);
    layout->addWidget(tool_bar_container);

    ElaToolBar* tool_bar = new ElaToolBar(this);
    setting_button       = new ElaToolButton(this);
    setting_button->setToolTip("设置");
    setting_button->setIcon(QIcon(":icons/images/better_setting.svg"));
    // setting_button->setElaIcon(ElaIconType::SendBack);
    tool_bar->addWidget(setting_button);
    tool_bar->setIconSize(QSize(16, 16));
    tool_bar_layout->addWidget(tool_bar);


    stack_container = new QStackedWidget(this);
    layout->addWidget(stack_container);

    stack_container->addWidget(signin);
    stack_container->addWidget(signup);
    stack_container->setCurrentIndex(0);
}

void Login::switch_to_signin() {
    hide_login_setting();
    stack_container->setCurrentIndex(0);
}

void Login::switch_to_signup() {
    hide_login_setting();
    stack_container->setCurrentIndex(1);
}

void Login::signin_clicked() {
    hide_login_setting();
    ElaMessageBar::success(
        ElaMessageBarType::TopRight, "login", "正在初始化,请稍后...", 2700, this);
    // if success,emit the signal!
    emit success_login();
}

void Login::setting_button_clicked() {
    int w = 320;
    int h = 427;
    this->login_setting->setFixedSize(QSize(w, h));
    auto p = this->geometry();
    int  x = p.x() + p.width() + 32;
    // avoid negative!
    int y = p.y() + std::abs((p.height() - h)) / 2;
    this->login_setting->setGeometry(QRect(x, y, w, h));
    // this->login_setting->moveToCenter();
    this->login_setting->show();
}

}   // namespace client
}   // namespace tang