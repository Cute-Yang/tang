
#pragma once
#include <QNetworkAccessManager>
#include "client_http_urls.h"
#include "client_cache.h"

namespace tang {
namespace client {
// Add all singleton function here,avoid namespace conflict!
struct ClientSingleton {
    static QNetworkAccessManager& get_network_manager_instance();
    static ClientHttpUrls& get_http_urls_instance();
   static  CacheUserInfo& get_cache_user_info_instance();
   static CacheWorkspaceData& get_cache_workspace_data_instance();
};
}   // namespace client
}   // namespace tang