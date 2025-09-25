
#pragma once
#include "client_cache.h"
#include "client_http_urls.h"
#include <QNetworkAccessManager>


namespace tang {
namespace client {
// Add all singleton function here,avoid namespace conflict!
struct ClientSingleton {
    static QNetworkAccessManager& get_network_manager_instance();
    static ClientHttpUrls&        get_http_urls_instance();
    static CacheUserInfo&         get_cache_user_info_instance();
    static CacheWorkspaceData&    get_cache_workspace_data_instance();
    static QString&                get_cache_file_dir();
};
}   // namespace client
}   // namespace tang