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
};
}   // namespace client
}   // namespace tang