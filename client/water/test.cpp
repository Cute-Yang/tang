#include <QFile>
#include <QDebug>
#include <QApplication>
#include "ElaWidget.h"
#include "ElaApplication.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    eApp->init();
    // 检查资源
    qDebug() << "Exists:" << QFile::exists(":/icons/images/cat.svg");
    ElaWidget w;
    w.setWindowIcon(QIcon(":/icons/images/cat.svg"));
    w.show();

    return app.exec();
}