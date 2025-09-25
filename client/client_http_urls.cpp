#include "client_http_urls.h"

namespace tang {
namespace client {
ClientHttpUrls::ClientHttpUrls(const QString& host_, int port_)
    : host(host_)
    , port(port_) {
    this->format_http_urls();
}

void ClientHttpUrls::set_server_value(const QString& host_, int port_) {
    host = host_;
    port = port_;
    this->format_http_urls();
}

void ClientHttpUrls::format_http_urls() {
    // initialize the login url!
    login_http_url = QString("http://%1:%2/api/v1/login").arg(host).arg(port);
    // signup url!
    signup_http_url = QString("http://%1:%2/api/v1/signup").arg(host).arg(port);
    // workspace url!
    workspace_http_url = QString("http://%1:%2/api/v1/get_workspace_names").arg(host).arg(port);
    workspace_content_http_url = QString("http://%1:%2/api/v1/get_file_infos").arg(host).arg(port);

    // download file!
    download_file_http_url = QString("http://%1:%2/api/v1/download_file").arg(host).arg(port);
}


const QString& ClientHttpUrls::get_login_url() {
    return login_http_url;
}

const QString& ClientHttpUrls::get_signup_url() {
    return signup_http_url;
}

const QString& ClientHttpUrls::get_workspace_url() {
    return workspace_http_url;
}

const QString& ClientHttpUrls::get_workspace_content_url() {
    return workspace_content_http_url;
}

const QString& ClientHttpUrls::get_download_file_url() {
    return download_file_http_url;
}

}   // namespace client
}   // namespace tang