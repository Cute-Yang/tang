#include "signin.h"
#include "QApplication"
#include "ElaApplication.h"
#include <qapplication.h>

int main(int argc,char* argv[]){
    QApplication app(argc,argv);
    eApp->init();
    // TangPdfWidget w;
    tang::client::SignIn w;
    w.show();
    return app.exec();
}