#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    //qint32 coord = (double)100 / (double)0.001;

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
