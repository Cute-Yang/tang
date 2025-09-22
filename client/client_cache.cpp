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


void CacheWorkspaceData::set_workspaces(std::vector<QString>&& names) {
    workspaces = std::move(names);
}

void CacheWorkspaceData::set_workspaces(const std::vector<QString>& names) {
    workspaces = names;
}

std::span<QString> CacheWorkspaceData::get_workspaces() {
    return std::span<QString>(workspaces.data(), workspaces.size());
}


void CacheWorkspaceData::set_workspace_show_names(std::vector<QString>&& names) {
    workspace_show_names = std::move(names);
}

void CacheWorkspaceData::set_workspace_show_names(const std::vector<QString>& names) {
    workspace_show_names = names;
}

std::span<QString> CacheWorkspaceData::get_workspace_show_names() {
    return std::span<QString>(workspace_show_names.data(), workspace_show_names.size());
}




}   // namespace client
}   // namespace tang