#pragma once
#include <ElaLineEdit.h>
#include <QAction>
#include "common/status.h"
#include <QJsonDocument>
#include <QNetworkReply>

namespace tang {
namespace client {
    struct RemoteFileInfo {
    QString          file_name;
    size_t           file_size;
    tang::common::FileKind file_type;
    QString          modify_time;
};
void switch_password_eye_style(ElaLineEdit* line_edit, QAction* action, bool hide);

QWidget* find_root_widget(QWidget* widget);

std::optional<QJsonDocument> get_json_document(QNetworkReply* reply);

}

}   // namespace tang