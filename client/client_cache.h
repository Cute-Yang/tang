#pragma once
#include "util.h"
#include <map>
#include <span>

namespace tang {
namespace client {
struct CacheUserInfo {
    QString user_name;
    QString email;
    uint8_t user_id;
    uint8_t vote_prioirty;
};

using workspace_file_infos_t = QList<RemoteFileInfo>;
struct CacheWorkspaceData {
    // like a file manager!
    std::map<QString, workspace_file_infos_t> workspace_file_infos;
    // the workspace names!
    QStringList workspaces;
    QStringList workspace_show_names;


    std::span<RemoteFileInfo> get_file_infos(const QString& workspace_name);
    bool                     delete_file_info(const QString& workspace_name, size_t i);
    void                      set_file_infos(const QString&                workspace_name,
                                             const workspace_file_infos_t& file_infos = {});
    void set_file_infos(const QString& workspace_name, workspace_file_infos_t&& file_infos = {});

    void               set_workspaces(QStringList&& names);
    void               set_workspaces(const QStringList& names);
    std::span<QString> get_workspaces();


    void               set_workspace_show_names(QStringList&& names);
    void               set_workspace_show_names(const QStringList& names);
    std::span<QString> get_workspace_show_names();

    size_t size();

    QString& get_workspace(size_t i);
    QString& get_workspace_show_name(size_t i);
};

}   // namespace client
}   // namespace tang