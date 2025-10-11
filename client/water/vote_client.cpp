#include "ElaApplication.h"
#include "QApplication"
#include "client.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    eApp->init();
    tang::client::Client client;
    return app.exec();
}