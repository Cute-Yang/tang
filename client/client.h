#pragma once
#include "login.h"
#include "main_window.h"
namespace tang {
namespace client {
class Client{
private:
    Login login;
    ClientMainWindow client_main_window;

public:
    Client();
    ~Client();

};
}
}