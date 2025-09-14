#pragma once
#include "ElaWidget.h"
#include "signin_ui.h"
namespace tang {
namespace client {
class SignIn:public ElaWidget{
    Q_OBJECT;


public:
    SignIn(QWidget* parent=nullptr);
    SigninUi* ui;
    
private slots:
    void password_eye_checked(bool hide);

    void signin_button_clicked();
    
};
}
}