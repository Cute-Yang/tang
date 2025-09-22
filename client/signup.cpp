#include "signup.h"
#include "ElaMessageBar.h"
#include "client_global_config.h"
#include "client_singleton.h"
#include "common/response_keys.h"
#include "common/status.h"
#include "util.h"
#include <QJsonObject>
#include <QNetworkReply>
#include <QUrlQuery>



using namespace tang::common;
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
    this->initialize_connects();
}


SignUp::~SignUp() {
    delete ui;
}

void SignUp::initialize_connects() {
    connect(ui->password_eye_action, &QAction::toggled, this, &SignUp::password_eye_checked);

    connect(ui->confirm_password_eye_action,
            &QAction::toggled,
            this,
            &SignUp::confirm_password_eye_checked);
    connect(ui->confirm_button, &ElaToolButton::clicked, this, &SignUp::on_signup_button_clicked);
}

void SignUp::password_eye_checked(bool hide) {
    switch_password_eye_style(ui->password_line_edit, ui->password_eye_action, hide);
}

void SignUp::confirm_password_eye_checked(bool hide) {
    switch_password_eye_style(
        ui->confirm_password_line_edit, ui->confirm_password_eye_action, hide);
}
void SignUp::process_signup_response(QNetworkReply* reply) {
    auto show_widget = find_root_widget(this);
    // process the response!
    if (reply->error() != QNetworkReply::NoError) {
        ElaMessageBar::error(ElaMessageBarType::TopRight,
                             "signup",
                             QString("网络异常:") + reply->errorString(),
                             ClientGlobalConfig::message_show_time,
                             show_widget);
        reply->deleteLater();
        return;
    }
    auto resp_data = reply->readAll();
    reply->deleteLater();
    QJsonParseError json_error;
    QJsonDocument   json_doc = QJsonDocument::fromJson(resp_data, &json_error);
    if (json_error.error != QJsonParseError::NoError) {
        ElaMessageBar::error(
            ElaMessageBarType::TopRight,
            "signup",
            QString("服务器返回数据格式错误! reason(%1)").arg(json_error.errorString()),
            ClientGlobalConfig::message_show_time,
            show_widget);
        return;
    }
    if (!json_doc.isObject()) {
        ElaMessageBar::error(ElaMessageBarType::TopRight,
                             "signup",
                             "服务器返回数据格式错误!",
                             ClientGlobalConfig::message_show_time,
                             show_widget);
        return;
    }
    auto    json_data = json_doc.object();
    uint8_t code      = json_data[PublicResponseJsonKeys::status_key].toInt();
    if (code != static_cast<uint8_t>(StatusCode::kSuccess)) {
        ElaMessageBar::error(ElaMessageBarType::TopRight,
                             "signup",
                             QString("注册失败 (reson:%1)")
                                 .arg(json_data[PublicResponseJsonKeys::message_key].toString()),
                             ClientGlobalConfig::message_show_time,
                             show_widget);
        return;
    }
    ElaMessageBar::success(ElaMessageBarType::TopRight,
                           "signup",
                           "注册成功,请返回登录!",
                           ClientGlobalConfig::message_show_time,
                           show_widget);
}

void SignUp::send_signup_http_req() {
    // send the http req!
    QNetworkRequest reqest(ClientSingleton::get_http_urls_instance().get_signup_url());
    reqest.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    // set the query params!
    auto      user_name = ui->user_line_edit->text();
    auto      password  = ui->password_line_edit->text();
    auto      email     = ui->email_line_edit->text();
    QUrlQuery query;
    query.addQueryItem("username", user_name);
    query.addQueryItem("password", password);
    query.addQueryItem("email", email);
    QByteArray     data    = query.toString(QUrl::FullyEncoded).toUtf8();
    auto&          manager = ClientSingleton::get_network_manager_instance();
    QNetworkReply* reply   = manager.post(reqest, data);

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        this->process_signup_response(reply);
    });
}


void SignUp::on_signup_button_clicked() {
    auto show_widget = find_root_widget(this);
    // get the data!
    auto user_name        = ui->user_line_edit->text();
    auto password         = ui->password_line_edit->text();
    auto confirm_password = ui->confirm_password_line_edit->text();
    auto email            = ui->email_line_edit->text();
    if (user_name.isEmpty()) {
        ElaMessageBar::warning(ElaMessageBarType::TopRight,
                               "signup",
                               "用户名不能为空!",
                               ClientGlobalConfig::message_show_time,
                               show_widget);
        return;
    }
    if (password.isEmpty()) {
        ElaMessageBar::warning(ElaMessageBarType::TopRight,
                               "signup",
                               "密码不能为空!",
                               ClientGlobalConfig::message_show_time,
                               show_widget);
        return;
    }
    if (confirm_password.isEmpty()) {
        ElaMessageBar::warning(ElaMessageBarType::TopRight,
                               "signup",
                               "确认密码不能为空!",
                               ClientGlobalConfig::message_show_time,
                               show_widget);
        return;
    }
    if (password != confirm_password) {
        ElaMessageBar::warning(ElaMessageBarType::TopRight,
                               "signup",
                               "两次输入的密码不一致!",
                               ClientGlobalConfig::message_show_time,
                               show_widget);
        return;
    }
    // no,here allow email is empty!
    if (email.isEmpty()) {
        // so just warning is ok!
        ElaMessageBar::warning(ElaMessageBarType::TopRight,
                               "signup",
                               "邮箱为空,将无法找回密码,请谨慎操作!",
                               ClientGlobalConfig::message_show_time,
                               show_widget);
        // return;
    }
    this->send_signup_http_req();
}

}   // namespace client
}   // namespace tang