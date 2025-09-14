#include "login.h"
#include "QApplication"
#include "ElaApplication.h"

int main(int argc,char* argv[]){
    QApplication app(argc,argv);
    eApp->init();
    // TangPdfWidget w;
    tang::client::Login w;
    w.show();
    return app.exec();
}