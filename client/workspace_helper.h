#pragma once
#include <QString>
#include <QStringList>

namespace tang {
namespace client {
struct WorkspacePathHelper {
    QStringList workspace_items;
    // the show name!
    QString workspace_show_name;
    QString workspace;
    WorkspacePathHelper() { workspace_items.reserve(8); }

    void set_workspace(const QString& workspace_) { workspace = workspace_; }
    void set_workspace_show_name(const QString& workspace_show_name_) {
        workspace_show_name = workspace_show_name_;
    }
    QString get_workspace_show_path() {
        if (workspace_items.size() == 0) {
            return QString("/%1").arg(workspace_show_name);
        }
        return QString("/%1/%2").arg(workspace_show_name).arg(workspace_items.join('/'));
    }
    QString get_workspace_path() {
        if (workspace_items.size() == 0) {
            return QString("%1").arg(workspace);
        }
        // attention,do not add this!
        // our path is relative
        return QString("%1/%2").arg(workspace).arg(workspace_items.join('/'));
    }

    void append(const QString& item) { workspace_items.push_back(item); }

    void append(QString&& item) { workspace_items.push_back(std::move(item)); }

    void clear() {
        workspace.clear();
        workspace_show_name.clear();
        workspace_items.clear();
    }

    size_t item_size() { return workspace_items.size(); }

    void pop_back_safe() {
        if (workspace_items.size() > 0) {
            workspace_items.pop_back();
        }
    }
};
}   // namespace client
}   // namespace tang
