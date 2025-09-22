#include "client_singleton.h"

namespace tang {
namespace client {

QNetworkAccessManager& ClientSingleton::get_network_manager_instance() {
    static QNetworkAccessManager instance;
    return instance;
}

ClientHttpUrls& ClientSingleton::get_http_urls_instance() {
    static ClientHttpUrls instance("localhost", 9000);
    return instance;
}

CacheUserInfo& ClientSingleton::get_cache_user_info_instance() {
    static CacheUserInfo instance;
    return instance;
}

}   // namespace client
}   // namespace tang