#include "mainwindow.h"
#include <QApplication>

#include <definitionbasicblocks.h>
#include <definitioncommandblock.h>


int main(int argc, char *argv[])
{
    qreal __qreal = 10/0.001;
    qint32 __qint32 = static_cast<qint32>(__qreal);

    AbstractDataBlock __adb;
    AbstractDataBlock __adb2;

    CellDataBlock __edb;
    __adb.Value(0,QVariant::fromValue(123));

    AbstractCommandBlock __acb;
    CylinderMonitorBlock __cmb;

    __edb = __adb;
    __edb = QVariant::fromValue(__adb).value<CellDataBlock>(); //invalid conversion
    __edb = QVariant::fromValue(static_cast<CellDataBlock>(__adb)).value<CellDataBlock>(); //valid conversion

    __adb = __acb; //vPtr wont be assigned

    __acb.Value(0,QVariant::fromValue(456));

    __adb = __acb; //valid assignment

    __adb.Value(0,QVariant::fromValue(567));
    __adb2 = __adb;
    *static_cast<AbstractDataBlock*>(&__acb) = __adb; //valid

    __acb = QVariant::fromValue(__adb).value<AbstractCommandBlock>(); //invalid assignment

    __adb = QVariant::fromValue(__acb).value<AbstractDataBlock>(); //invalid
    __edb = QVariant::fromValue(__acb).value<CellDataBlock>(); //invalid

    quint16* anchor = reinterpret_cast<quint16*>(&__adb);
    quint16 value = anchor[0]; // post shift

    size_t sz = sizeof(CellDataBlock);
    sz = sizeof(AbstractDataBlock);
    sz = sizeof(ExtendedCommandBlock);

    uint __mask = 0x0001 << 5;

    qputenv("QT_IM_MODULE",QByteArray("qtvirtualkeyboard"));
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
