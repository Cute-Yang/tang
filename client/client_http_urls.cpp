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

    // upload file
    upload_file_http_url = QString("http://%1:%2/api/v1/upload_file").arg(host).arg(port);
    is_file_exist_http_url =
        QString("http://%1:%2/api/v1/is_specify_file_exist").arg(host).arg(port);
    create_dir_http_url    = QString("http://%1:%2/api/v1/create_dir").arg(host).arg(port);
    delete_file_http_url   = QString("http://%1:%2/api/v1/delete_file").arg(host).arg(port);
    rename_file_http_url   = QString("http://%1:%2/api/v1/move_file").arg(host).arg(port);
    online_voters_http_url = QString("http://%1:%2/api/v1/get_online_voters").arg(host).arg(port);
    create_vote_http_url   = QString("http://%1:%2/api/v1/create_vote").arg(host).arg(port);

    vote_num_http_url = QString("http://%1:%2/api/v1/get_vote_num").arg(host).arg(port);
    chunk_vote_data_http_url =
        QString("http://%1:%2/api/v1/get_chunk_vote_data").arg(host).arg(port);

    participate_vote_num_http_url =
        QString("http://%1:%2/api/v1/get_participate_vote_num").arg(host).arg(port);
    chunk_participate_vote_data_http_url =
        QString("http://%1:%2/api/v1/get_chunk_participate_vote_data").arg(host).arg(port);

    ws_chat_client_url = QString("ws://localhost:9000/api/v1/chat?user_id=%1&user_name=%2");
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
const QString& ClientHttpUrls::get_upload_file_url() {
    return upload_file_http_url;
}

const QString& ClientHttpUrls::get_is_file_exist_url() {
    return is_file_exist_http_url;
}

const QString& ClientHttpUrls::get_create_new_dir_url() {
    return create_dir_http_url;
}

const QString& ClientHttpUrls::get_delete_file_url() {
    return delete_file_http_url;
}

const QString& ClientHttpUrls::get_rename_file_url() {
    return rename_file_http_url;
}

const QString& ClientHttpUrls::get_online_voters_url() {
    return online_voters_http_url;
}

const QString& ClientHttpUrls::get_create_vote_http_url() {
    return create_vote_http_url;
}

const QString& ClientHttpUrls::get_vote_num_url() {
    return vote_num_http_url;
}
const QString& ClientHttpUrls::get_chunk_vote_data_url() {
    return chunk_vote_data_http_url;
}

const QString& ClientHttpUrls::get_participate_vote_num_url() {
    return participate_vote_num_http_url;
}
const QString& ClientHttpUrls::get_participate_chunk_vote_data_url() {
    return chunk_participate_vote_data_http_url;
}

QString ClientHttpUrls::get_ws_chat_client_url(uint32_t user_id, const QString& user_name) {
    return ws_chat_client_url.arg(user_id).arg(user_name);
}

}   // namespace client
}   // namespace tang