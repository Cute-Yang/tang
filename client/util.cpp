#include "util.h"
#include "client_singleton.h"
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrlQuery>

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

void show_and_raise(QWidget* widget) {
    widget->show();
    widget->raise();
    widget->activateWindow();
}


QNetworkReply* send_http_req_with_json_data(const QJsonObject& json_data, const QString& url_str) {
    QUrl            url(url_str);
    QNetworkRequest request(url_str);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    // prepare json data
    QJsonDocument json_doc(json_data);
    QByteArray    json_bytes = json_doc.toJson(QJsonDocument::Compact);
    auto          reply = ClientSingleton::get_network_manager_instance().post(request, json_bytes);
    return reply;
}

QNetworkReply* send_http_req_with_form_data(const QUrlQuery& query,const QString& url_str){
      QUrl            url(url_str);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    // no need any params!
    auto&     manager = ClientSingleton::get_network_manager_instance();

    QByteArray     query_data = query.toString(QUrl::FullyEncoded).toUtf8();
    QNetworkReply* reply      = manager.post(request, query_data);
    return reply;
}

std::string format_time(const std::chrono::system_clock::time_point& tp) {
    // 转换为 time_t
    std::time_t t = std::chrono::system_clock::to_time_t(tp);

    // 转换为本地时间的 tm 结构
    std::tm tm_snapshot;
#ifdef _WIN32
    localtime_s(&tm_snapshot, &t);
#else
    localtime_r(&tt, &tm_snapshot);   // POSIX
#endif
    // 使用 stringstream 和 put_time 格式化
    std::ostringstream oss;
    oss << std::put_time(&tm_snapshot, "%Y-%m-%d %H:%M:%S");

    return oss.str();
}

QString get_vote_status_display_str(common::VoteStatus status) {
    switch (status) {
    case common::VoteStatus::kReady: return QString("投票中"); break;
    case common::VoteStatus::kFinished: return QString("已完成"); break;
    default: return QString("已作废"); break;
    }
}



}   // namespace client
}   // namespace tang