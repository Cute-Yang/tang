#include "client_singleton.h"
#include <QDir>
#include <QStandardPaths>


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
    // only for test!
    instance.user_name     = "陈静";
    instance.email         = "chenjing@qq.com";
    instance.user_id       = 9;
    instance.vote_prioirty = 1;


    return instance;
}

CacheWorkspaceData& ClientSingleton::get_cache_workspace_data_instance() {
    static CacheWorkspaceData instance;
    return instance;
}

// initialize only for one,if not exit,just create it!
QString get_cache_file_dir_impl() {
    QString home_dir   = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QString cache_path = QString("%1/tang_meeting_cache").arg(home_dir);
    QDir    dd(cache_path);
    if (!dd.exists()) {
        if (!dd.mkpath(cache_path)) {
            return "";
        }
    }
    return cache_path;
}

QString& ClientSingleton::get_cache_file_dir() {
    static QString cache_path = get_cache_file_dir_impl();
    return cache_path;
}

}   // namespace client
}   // namespace tang