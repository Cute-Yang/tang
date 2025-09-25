#include "main_window.h"
#include "QApplication"
#include "ElaApplication.h"
#include <QThread>

int main(int argc,char* argv[]){
    QApplication app(argc,argv);
    eApp->init();
    // TangPdfWidget w;
    tang::client::ClientMainWindow w;
    // qDebug() << "the main thread is " << QThread::currentThreadId() << " main" << QThread::isMainThread();
    w.show();
    return app.exec();
}