#include "client_cache.h"
namespace tang {
namespace client {
std::span<RemoteFileInfo> CacheWorkspaceData::get_file_infos(const QString& workspace_name) {
    auto iter = workspace_file_infos.find(workspace_name);
    if (iter == workspace_file_infos.end()) {
        return {};
    }
    auto& vec = iter->second;
    return std::span(vec.data(), vec.size());
}

void CacheWorkspaceData::set_file_infos(const QString&                     workspace_name,
                                        const std::vector<RemoteFileInfo>& file_infos) {
    // just create an empty vector!
    workspace_file_infos[workspace_name] = file_infos;
}

void CacheWorkspaceData::set_file_infos(const QString&                workspace_name,
                                        std::vector<RemoteFileInfo>&& file_infos) {
    workspace_file_infos[workspace_name] = std::move(file_infos);
}


void CacheWorkspaceData::set_workspace_names(std::vector<QString>&& names) {
    workspace_names = std::move(names);
}

void CacheWorkspaceData::set_workspace_names(const std::vector<QString>& names) {
    workspace_names = names;
}




}   // namespace client
}   // namespace tang