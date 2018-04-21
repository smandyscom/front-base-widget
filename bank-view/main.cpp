#include "mainwindow.h"
#include <QApplication>

#include <definitionbasicblocks.h>
#include <definitioncommandblock.h>

int main(int argc, char *argv[])
{
    AbstractDataBlock __adb;
    __adb.Value(0,QVariant::fromValue(123));

    quint16* anchor = reinterpret_cast<quint16*>(&__adb);
    quint16 value = anchor[2]; // post shift

    size_t sz = sizeof(AbstractDataBlock);
    sz = sizeof(ExtendedCommandBlock);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
