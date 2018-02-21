#ifndef MODBUSCHANNELTEST_H
#define MODBUSCHANNELTEST_H

#include <task.h>
#include <modbuschannel.h>
#include <QMetaType>

#include <QVariant>
#include <utilities.h>

#include <baselayerdefinitions.h>
using namespace BaseLayer;

struct theObject
{
    WORD field1;
    LONG field2;//32bits
    LONG filed3;

//    theObject(WORD arg1,LONG arg2,LONG arg3):
//        field1(arg1),
//        field2(arg2),
//        filed3(arg3){}
};
Q_DECLARE_METATYPE(theObject)

class ModbusChannelTest : public Task
{
public:
    enum addressTable
    {
        HEADER = 0x00000000,
        BODY = 0x00010000,
    };
    Q_ENUM(addressTable)

    ModbusChannelTest();

protected:
    ModbusChannel* channel;
};

#endif // MODBUSCHANNELTEST_H
