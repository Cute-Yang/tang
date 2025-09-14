#include "login_setting.h"
#include "QApplication"
#include "ElaApplication.h"

int main(int argc,char* argv[]){
    QApplication app(argc,argv);
    eApp->init();
    // TangPdfWidget w;
    tang::client::LoginSetting w;
    w.show();
    return app.exec();
}