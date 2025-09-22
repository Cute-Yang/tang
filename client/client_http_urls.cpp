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
    login_http_url = QString("http://%1:%2/api/v1/login").arg(host).arg(port);
}


QString& ClientHttpUrls::get_login_url() {
    return login_http_url;
}

const QString& ClientHttpUrls::get_login_url() const {
    return login_http_url;
}

}   // namespace client
}   // namespace tang