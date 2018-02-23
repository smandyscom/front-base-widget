#ifndef MODBUSCHANNELTEST_H
#define MODBUSCHANNELTEST_H

#include <task.h>
#include <modbuschannel.h>
#include <QMetaType>

#include <QVariant>
#include <utilities.h>

#include <baselayerdefinitions.h>
using namespace BaseLayer;

#include <QModbusTcpClient>
#pragma pack(1)
struct theObject
{
    WORD field1;//2bytes
    //WORD field10;
    LONG field2;//4bytes
    //LONG filed3;//4bytes

//    theObject(WORD arg1,LONG arg2,LONG arg3):
//        field1(arg1),
//        field2(arg2),
//        filed3(arg3){}
}__attribute__((packed,aligned (1)));

struct alignmentObject
{
   char x;
   int a;
}__attribute__((packed,aligned (1)));


Q_DECLARE_METATYPE(theObject)

class ModbusChannelTest : public Task
{
public:
    enum addressTable
    {
        HEADER = 0x00020000,
        BODY = 0x00000001,
        LOOP_HEADER=0x00040004,
        LOOP = 0x00000005,
    };
    Q_ENUM(addressTable)

    ModbusChannelTest();

protected:
    ModbusChannel* channel;
    ModbusSerializedClient *client;
    QModbusTcpClient* driver;
public slots:
    virtual void run();
    void onUpdated(UpdateEvent *);
    void onDriverConnected(QModbusDevice::State state);
protected:
    QVariant cachedValue;
    QVariant cachedBool;
};

#endif // MODBUSCHANNELTEST_H
