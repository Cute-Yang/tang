#include "test_plot.h"
#include "QApplication"
#include <QPieSeries>
#include <QPieSlice>
#include <QChartView>
#include <QTimer>

int main(int argc,char* argv[]){
    QApplication app(argc,argv);
    // TangPdfWidget w;
    MainWindow w;
    w.show();
    return app.exec();

}