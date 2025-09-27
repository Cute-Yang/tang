#include "util.h"
#include <QNetworkReply>
#include <QNetworkRequest>
#include "client_singleton.h"
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
QWidget* find_root_widget(QWidget* widget) {
    QWidget* parent = widget;
    while (parent->parentWidget()) {
        parent = parent->parentWidget();
    }
    return parent;
}

std::optional<QJsonDocument> get_json_document(QNetworkReply* reply) {
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << reply->errorString();
        reply->deleteLater();
        return {};
    }
    auto resp = reply->readAll();
    // avoid memory leak!
    reply->deleteLater();
    // parse it from the json!
    QJsonParseError parse_error;
    QJsonDocument   document = QJsonDocument::fromJson(resp, &parse_error);
    if (parse_error.error != QJsonParseError::NoError) {
        return {};
    }
    return document;
}

std::pair<size_t, size_t> remove_path_sep(const QString& folder_path) {
    auto l = 0;
    auto r = folder_path.size() - 1;
    while (folder_path[l] == '/' || folder_path[l] == '\\') {
        ++l;
    }
    while (folder_path[r] == '/' || folder_path[r] == '\\') {
        --r;
    }
    return {l, r};
}

void show_and_raise(QWidget* widget){
    widget->show();
    widget->raise();
    widget->activateWindow();
}


QNetworkReply* send_http_req_with_json_data(const QJsonObject& json_data,const QString& url_str){
    QUrl url(url_str);
    QNetworkRequest request(url_str);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    // prepare json data
    QJsonDocument json_doc(json_data);
    QByteArray    json_bytes = json_doc.toJson(QJsonDocument::Compact);
    auto          reply = ClientSingleton::get_network_manager_instance().post(request, json_bytes);
    return reply;
}
}   // namespace client
}   // namespace tang