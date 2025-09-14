#include "signin.h"
#include "util.h"

namespace tang {
namespace client {
SignIn::SignIn(QWidget* parent):ElaWidget(parent),ui(new SigninUi){
    ui->setup_ui(this);
    this->setWindowTitle("login");
    this->setWindowIcon(QIcon(":/icons/images/xiaoxiong.svg"));
    this->setWindowIconText("~");

    // this->moveToCenter();
    // this->setFixedSize(QSize(350,520));
    connect(ui->password_eye_action,&QAction::toggled,this,&SignIn::password_eye_checked);

}

void SignIn::password_eye_checked(bool hide){
    switch_password_eye_style(ui->password_line_edit,ui->password_eye_action,hide);
    
}

void SignIn::signin_button_clicked(){
    QString user_name = ui->user_line_edit->text();
    if(user_name.isEmpty()){
        // ElaMessageBar::warning("")
    }
}
}
}