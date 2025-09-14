#pragma once
#include <QString>

namespace tang {
namespace client {
struct ClientUserInfo{
  //user_name
  QString user_name;
  QString user_email;
};

struct ServerAddressInfo{
    QString server_addr;
    int server_http_port;
    int server_websocket_port;
};
}
}