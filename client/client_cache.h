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


struct CacheWorkspaceData {
    // like a file manager!
    std::map<QString, std::vector<RemoteFileInfo>> workspace_file_infos;
    // the workspace names!
    std::vector<QString> workspace_names;

    std::span<RemoteFileInfo> get_file_infos(const QString& workspace_name);
    void                      set_file_infos(const QString&                     workspace_name,
                                             const std::vector<RemoteFileInfo>& file_infos = {});
    void                      set_file_infos(const QString&                workspace_name,
                                             std::vector<RemoteFileInfo>&& file_infos = {});
    
    void set_workspace_names(std::vector<QString>&& names) ;
    void set_workspace_names(const std::vector<QString>& names);
    std::span<QString> get_workspace_names() {
        return std::span(workspace_names.data(), workspace_names.size());
    }
};

}   // namespace client
}   // namespace tang