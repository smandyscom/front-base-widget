#include <QCoreApplication>
#include <../../bank-view/baselayerdefinitions.h>
using namespace BaseLayer;
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    ModbusDriverAddress address1;
    address1.channelAddress=0;
    address1.registerTypeBitIndex=0;
    address1.registerAddress=0;
    ModbusDriverAddress address2;
    address2.channelAddress=0;
    address2.registerTypeBitIndex=0;
    address2.registerAddress=0xff;
    DummyDriverAddress address3;

    quint32 v = 0;

    size_t sz = sizeof(address1);
    size_t sz2 = sizeof(address3);
    size_t sz3 = sizeof(v);
    bool result = (address1 == address2);


    return a.exec();
}
