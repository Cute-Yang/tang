#include "display_pdf_file.h"
#include "QApplication"
#include "ElaApplication.h"

int main(int argc,char* argv[]){
    QApplication app(argc,argv);
    eApp->init();
    // TangPdfWidget w;
    tang::client::DisplayPdf w;
    w.show();
    return app.exec();
}