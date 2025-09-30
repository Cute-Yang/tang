#include "client_cache.h"
namespace tang {
namespace client {
std::span<RemoteFileInfo> CacheWorkspaceData::get_file_infos(const QString& workspace_name) {
    auto iter = workspace_file_infos.find(workspace_name);
    if (iter == workspace_file_infos.end()) {
        return {};
    }
    auto& data = iter->second;
    return std::span(data.data(), data.size());
}

bool CacheWorkspaceData::delete_file_info(const QString& workspace_name, size_t i) {
    auto iter = workspace_file_infos.find(workspace_name);
    if (iter == workspace_file_infos.end()) {
        return false;
    }
    auto& data = iter->second;

    if (i >= data.size()) {
        return false;
    }
    data.removeAt(i);
    return true;
}


void CacheWorkspaceData::set_file_infos(const QString&                workspace_name,
                                        const workspace_file_infos_t& file_infos) {
    // just create an empty vector!
    workspace_file_infos[workspace_name] = file_infos;
}

void CacheWorkspaceData::set_file_infos(const QString&           workspace_name,
                                        workspace_file_infos_t&& file_infos) {
    workspace_file_infos[workspace_name] = std::move(file_infos);
}


void CacheWorkspaceData::set_workspaces(QStringList&& names) {
    workspaces = std::move(names);
}

void CacheWorkspaceData::set_workspaces(const QStringList& names) {
    workspaces = names;
}

std::span<QString> CacheWorkspaceData::get_workspaces() {
    return std::span<QString>(workspaces.data(), workspaces.size());
}


void CacheWorkspaceData::set_workspace_show_names(QStringList&& names) {
    workspace_show_names = std::move(names);
}

void CacheWorkspaceData::set_workspace_show_names(const QStringList& names) {
    workspace_show_names = names;
}

std::span<QString> CacheWorkspaceData::get_workspace_show_names() {
    return std::span<QString>(workspace_show_names.data(), workspace_show_names.size());
}




}   // namespace client
}   // namespace tang