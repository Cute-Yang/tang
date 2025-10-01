#pragma once
#include <QString>

namespace tang {
namespace client {

class ClientHttpUrls {
private:
    QString host;
    int     port;
    // store all the url,avoid re format!!
    QString login_http_url;
    QString signup_http_url;
    QString workspace_http_url;
    QString workspace_content_http_url;
    QString download_file_http_url;
    QString upload_file_http_url;
    QString is_file_exist_http_url;
    QString create_dir_http_url;
    QString rename_file_http_url;
    QString delete_file_http_url;
    QString online_voters_http_url;
    QString create_vote_http_url;

    void format_http_urls();

public:
    explicit ClientHttpUrls(const QString& host_ = "localhost", int port_ = 9000);

    void set_server_value(const QString& host_, int port_);
    // heng, only get const ref?
    const QString& get_login_url();
    const QString& get_signup_url();
    const QString& get_workspace_url();
    const QString& get_workspace_content_url();
    const QString& get_download_file_url();
    const QString& get_upload_file_url();
    const QString& get_is_file_exist_url();
    const QString& get_create_new_dir_url();
    const QString& get_rename_file_url();
    const QString& get_delete_file_url();
    const QString& get_online_voters_url();
    const QString& get_create_vote_http_url();
};
}   // namespace client
}   // namespace tang