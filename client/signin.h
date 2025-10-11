#pragma once
#include "ElaWidget.h"
#include "signin_ui.h"
#include <QNetworkReply>
namespace tang {
namespace client {
class SignIn : public ElaWidget {
    Q_OBJECT
public:
    SignIn(QWidget* parent = nullptr);
    SigninUi* ui;
    ~SignIn();
    void send_login_http_req();
    void process_login_response(QNetworkReply* reply);
    void initialize_connects();
signals:
    void success_login();
private slots:
    void password_eye_checked(bool hide);
    void on_signin_button_clicked();
};
}   // namespace client
}   // namespace tang