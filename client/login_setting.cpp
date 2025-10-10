#include "login_setting.h"
#include "ElaMessageBar.h"
#include "client_global_config.h"
#include "client_singleton.h"


namespace tang {
namespace client {
LoginSetting::LoginSetting(QWidget* parent)
    : ElaWidget(parent)
    , ui(new LoginSettingUi()) {
    ui->setup_ui(this);
    // this->setFixedSize(QSize(350, 520));
    this->setWindowTitle("setting");
    this->setWindowIcon(QIcon(":/icons/images/yellow_butterfly.svg"));
    this->moveToCenter();
    this->initialize_connects();
    this->setWindowButtonFlags(ElaAppBarType::CloseButtonHint | ElaAppBarType::MinimizeButtonHint);
}


LoginSetting::~LoginSetting() {
    delete ui;
}

void LoginSetting::initialize_connects() {
    // initialie the connects!
    connect(ui->confirm_button,
            &ElaToolButton::clicked,
            this,
            &LoginSetting::on_confirm_button_clicked);
}

void LoginSetting::on_confirm_button_clicked() {
    // get the data!
    auto server_addr = ui->server_addr_line_edit->text();
    auto server_port = ui->server_http_port_line_edit->text();
    if (server_addr.isEmpty()) {
        ElaMessageBar::warning(ElaMessageBarType::TopRight,
                               "setting",
                               "服务器地址不能为空!",
                               ClientGlobalConfig::message_show_time,
                               this);
        return;
    }
    if (server_port.isEmpty()) {
        ElaMessageBar::warning(ElaMessageBarType::TopRight,
                               "setting",
                               "服务器端口不能为空!",
                               ClientGlobalConfig::message_show_time,
                               this);
        return;
    }
    // set to singleton!
    auto& http_urls = ClientSingleton::get_http_urls_instance();
    http_urls.set_server_value(server_addr, server_port.toUInt());

    ElaMessageBar::success(ElaMessageBarType::TopRight,
                           "setting",
                           "设置成功!",
                           ClientGlobalConfig::message_show_time,
                           this);
}
}   // namespace client
}   // namespace tang