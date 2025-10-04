#include "signin.h"
#include "ElaMessageBar.h"
#include "ElaToolButton.h"
#include "client_global_config.h"
#include "client_singleton.h"
#include "common/response_keys.h"
#include "common/status.h"
#include "util.h"
#include <QJsonDocument>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QjsonObject>

using namespace tang::common;
namespace tang {
namespace client {
struct LoginResJsonValidator {
    static bool validate(const QJsonObject& json_data) {
        // validate the keys are all exist!
        for (size_t i = 0; i < LoginResponseJsonKeys::keys.size(); ++i) {
            if (!json_data.contains(LoginResponseJsonKeys::keys[i])) {
                return false;
            }
        }
        // validate the type is right?
        if (!json_data[LoginResponseJsonKeys::user_name_key].isString()) {
            return false;
        }

        if (!json_data[LoginResponseJsonKeys::email_key].isString()) {
            return false;
        }

        // fuck,the json in Qt,int is double???
        if (!json_data[LoginResponseJsonKeys::user_id_key].isDouble()) {
            return false;
        }
        if (!json_data[LoginResponseJsonKeys::vote_priority_key].isDouble()) {
            return false;
        }
        return true;
    }
};

SignIn::SignIn(QWidget* parent)
    : ElaWidget(parent)
    , ui(new SigninUi) {
    ui->setup_ui(this);
    this->setWindowTitle("login");
    this->setWindowIcon(QIcon(":/icons/images/xiaoxiong.svg"));
    this->setWindowIconText("~");
    this->initialize_connects();
}

SignIn::~SignIn() {
    delete ui;
}

void SignIn::initialize_connects() {
    connect(ui->password_eye_action, &QAction::toggled, this, &SignIn::password_eye_checked);
    connect(ui->sign_in_button, &ElaToolButton::clicked, this, &SignIn::on_signin_button_clicked);
}

void SignIn::password_eye_checked(bool hide) {
    switch_password_eye_style(ui->password_line_edit, ui->password_eye_action, hide);
}


void SignIn::process_login_response(QNetworkReply* reply) {
    auto show_widget = find_root_widget(this);
    auto document    = get_json_document(reply);
    if (!document) {
        ElaMessageBar::error(ElaMessageBarType::TopRight,
                             "login",
                             "网络异常/返回格式错误!",
                             ClientGlobalConfig::message_show_time,
                             show_widget);
        return;
    }

    auto&       document_value = document.value();
    QJsonObject json_data      = document_value.object();
    // here the default!add a public key conf!
    auto status = json_data[PublicResponseJsonKeys::status_key].toInt();
    if (status != static_cast<int>(StatusCode::kSuccess)) {
        // very nice!
        ElaMessageBar::error(ElaMessageBarType::TopRight,
                             "login",
                             QString("登录失败 (reason:%1)")
                                 .arg(json_data[PublicResponseJsonKeys::message_key].toString()),
                             ClientGlobalConfig::message_show_time,
                             show_widget);
        return;
    }
    if (!LoginResJsonValidator::validate(json_data)) {
        ElaMessageBar::error(ElaMessageBarType::TopRight,
                             "login",
                             QString("登录失败 (reason:服务器返回数据异常)"),
                             ClientGlobalConfig::message_show_time,
                             show_widget);
        return;
    }
    // then set the data!
    auto& cache_user_info = ClientSingleton::get_cache_user_info_instance();
    cache_user_info.user_name = json_data[LoginResponseJsonKeys::user_name_key].toString();
    cache_user_info.email     = json_data[LoginResponseJsonKeys::email_key].toString();
    cache_user_info.user_id =
        static_cast<uint32_t>(json_data[LoginResponseJsonKeys::user_id_key].toInt());
    cache_user_info.vote_prioirty =
        static_cast<uint8_t>(json_data[LoginResponseJsonKeys::vote_priority_key].toInt());
    // cache_user_info.print_data();

    ElaMessageBar::success(ElaMessageBarType::TopRight,
                           "login",
                           "正在初始化,请稍后 (✿◠‿◠)...",
                           ClientGlobalConfig::message_show_time,
                           show_widget);
    // if success,emit the signal!
    emit success_login();
}

void SignIn::send_login_http_req() {
    auto            show_widget = find_root_widget(this);
    QNetworkRequest reqest(ClientSingleton::get_http_urls_instance().get_login_url());
    reqest.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    // set the query params!
    QUrlQuery query;
    auto      user_name = ui->user_line_edit->text();
    auto      password  = ui->password_line_edit->text();
    query.addQueryItem("username", user_name);
    query.addQueryItem("password", password);

    QByteArray     data    = query.toString(QUrl::FullyEncoded).toUtf8();
    auto&          manager = ClientSingleton::get_network_manager_instance();
    QNetworkReply* reply   = manager.post(reqest, data);
    ElaMessageBar::information(ElaMessageBarType::TopRight,
                               "login",
                               "正在连接服务器,请稍后...",
                               ClientGlobalConfig::message_show_time,
                               show_widget);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        this->process_login_response(reply);
    });
}

void SignIn::on_signin_button_clicked() {
    // show message on the root!
    auto show_widget = find_root_widget(this);
    auto user_name   = ui->user_line_edit->text();
    auto password    = ui->password_line_edit->text();
    if (user_name.isEmpty()) {
        ElaMessageBar::warning(ElaMessageBarType::TopRight,
                               "login",
                               "用户名不能为空!",
                               ClientGlobalConfig::message_show_time,
                               show_widget);
        return;
    }
    if (password.isEmpty()) {
        ElaMessageBar::warning(ElaMessageBarType::TopRight,
                               "login",
                               "密码不能为空!",
                               ClientGlobalConfig::message_show_time,
                               show_widget);
        return;
    }
    this->send_login_http_req();
}
}   // namespace client
}   // namespace tang