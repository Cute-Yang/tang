#pragma once
#include "ElaWindow.h"
namespace tang {
namespace client {
class ClientMainWindow : public ElaWindow{
Q_OBJECT
private:
    void init_page();

public:
    ClientMainWindow(QWidget* parent = nullptr);
    ~ClientMainWindow(){}


// private:
    
};
}
}