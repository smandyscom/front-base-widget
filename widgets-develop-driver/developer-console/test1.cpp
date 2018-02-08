#include <../../bank-view/baselayerdefinitions.h>
using namespace BaseLayer;
void test1()
{
    ModbusDriverAddress addr1(0x00151001); // bit index , 5 , mask 0x20
    ModbusDriverAddress addr2(0x01261002); // bit inded , 6 , mask 0x40
    ModbusDriverAddress addr3(addr1);

    ModbusDriverAddress addr4 = addr1;
//    ModbusDriverAddress address1;
//    address1.channelAddress=0;
//    address1.registerTypeBitIndex=0;
//    address1.registerAddress=0;
//    ModbusDriverAddress address2;
//    address2.channelAddress=0;
//    address2.registerTypeBitIndex=0;
//    address2.registerAddress=0xff;
//    DummyDriverAddress address3;

//    quint32 v = 0;

//    size_t sz = sizeof(address1);
//    size_t sz2 = sizeof(address3);
//    size_t sz3 = sizeof(v);
    bool result = (addr1 == addr2);
    result = addr1 == addr3;
    result = addr2 == addr3;

    QModbusDataUnit::RegisterType tp = addr1.getRegisterType();
    tp = addr2.getRegisterType();
    quint8 index = addr1.getBitIndex();
    index = addr2.getBitIndex();

    addr1.setRegisterType(QModbusDataUnit::Coils);
    tp = addr1.getRegisterType();

    addr1.setBitIndex(3); //bit index 3 , 0x08
    index = addr1.getBitIndex();


    //---------Bitwise mask
    uint mask1 = addr1.toBitwiseMask();
    uint mask2 = addr2.toBitwiseMask();
}
