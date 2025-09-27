#pragma once
#include "common/status.h"
#include <ElaLineEdit.h>
#include <QAction>
#include <QJsonDocument>
#include <QNetworkReply>


namespace tang {
namespace client {
struct RemoteFileInfo {
    QString                file_name;
    size_t                 file_size;
    tang::common::FileKind file_type;
    QString                modify_time;
};
void switch_password_eye_style(ElaLineEdit* line_edit, QAction* action, bool hide);

QWidget* find_root_widget(QWidget* widget);

std::optional<QJsonDocument> get_json_document(QNetworkReply* reply);

std::pair<size_t, size_t> remove_path_sep(const QString& file_path);

void show_and_raise(QWidget* widget);

QNetworkReply* send_http_req_with_json_data(const QJsonObject& json_data, const QString& url_str);
}   // namespace client
}   // namespace tang