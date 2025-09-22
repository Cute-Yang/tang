#include "login.h"
#include "ElaMessageBar.h"
#include "ElaToolBar.h"
#include "client_global_config.h"
#include "client_singleton.h"
#include "common/response_keys.h"
#include "common/status.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QUrlQuery>



using namespace tang::common;
namespace tang {
namespace client {
// a simple validator for the login response!
struct LoginResJsonValidator {
    static bool validate(const QJsonObject& json_data) {
        //validate the keys are all exist!
        for (size_t i = 0; i < LoginResponseJsonKeys::keys.size(); ++i) {
            if (!json_data.contains(LoginResponseJsonKeys::keys[i])) {
                return false;
            }
        }
        //validate the type is right?
        if(json_data[LoginResponseJsonKeys::user_name_key].type()!=QJsonValue::String){
            return false;
        }
        
        return true;
    }
};


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
}

Login::~Login() {
    delete login_setting;
}

void Login::initialize_connects() {
    // connect!
    connect(signin->ui->sign_up_button, &ElaToolButton::clicked, this, &Login::switch_to_signup);
    connect(signup->ui->back_button, &ElaToolButton::clicked, this, &Login::switch_to_signin);

    connect(signin->ui->sign_in_button,
            &ElaToolButton::clicked,
            this,
            &Login::on_signin_button_clicked);
    connect(setting_button, &ElaToolButton::clicked, this, &Login::on_setting_button_clicked);
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

void Login::send_login_http_req() {
    QNetworkRequest reqest(ClientSingleton::get_http_urls_instance().get_login_url());
    reqest.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    // set the query params!
    QUrlQuery query;
    query.addQueryItem("username", signin->ui->user_line_edit->text());
    query.addQueryItem("password", signin->ui->password_line_edit->text());

    QByteArray     data    = query.toString(QUrl::FullyEncoded).toUtf8();
    auto&          manager = ClientSingleton::get_network_manager_instance();
    QNetworkReply* reply   = manager.post(reqest, data);

    connect(reply, &QNetworkReply::finished, this, [this, reply] {
        auto resp = reply->readAll();
        reply->deleteLater();
        // parse it from the json!
        QJsonParseError parse_error;
        QJsonDocument   document = QJsonDocument::fromJson(resp, &parse_error);
        if (parse_error.error != QJsonParseError::NoError) {
            ElaMessageBar::error(ElaMessageBarType::TopRight,
                                 "login",
                                 parse_error.errorString(),
                                 ClientGlobalConfig::message_show_time,
                                 this);
            return;
        }
        QJsonObject json_data = document.object();
        qDebug() << json_data;
        // constexpr
        auto status = json_data["status"].toInt();
        if (status != static_cast<int>(StatusCode::kSuccess)) {
            ElaMessageBar::error(ElaMessageBarType::TopRight,
                                 "login",
                                 QString("登录失败 (reason:%1)").arg(get_status_str(status)),
                                 2700,
                                 this);
        }
        // then set the data!
        auto& cache_user_info = ClientSingleton::get_cache_user_info_instance();

        ElaMessageBar::success(
            ElaMessageBarType::TopRight, "login", "正在初始化,请稍后...", 2700, this);
        // if success,emit the signal!
        emit success_login();
    });
}
void Login::on_signin_button_clicked() {
    hide_login_setting();
    this->send_login_http_req();
}

void Login::on_setting_button_clicked() {
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