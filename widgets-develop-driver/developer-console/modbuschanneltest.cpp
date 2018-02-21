#include "modbuschanneltest.h"
#include <QDebug>

ModbusChannelTest::ModbusChannelTest()
{
    //channel = new ModbusChannel();

    QVariant value;

    //qRegisterMetaType<theObject>();
    //int id = qMetaTypeId<theObject>();

    theObject ob = {1,2,3};
    value.setValue(ob);

    theObject ob2 = value.value<theObject>();

    reinterpret_cast<quint16*>(value.data())[0]=0xeeee;
    reinterpret_cast<quint16*>(value.data())[1]=0xffff;

    theObject ob3 = value.value<theObject>();

    //!
    //! \brief sz0
    //! size function test, matched with each other
    //!
    qDebug() << sizeof(bool);
    qDebug() << sizeof(uint);
    qDebug() << sizeof(ulong);
    qDebug() << sizeof(WORD);
    qDebug() << sizeof(LONG);
    size_t sz0 = sizeof(ob);
    size_t sz = utilities::sizeOf(value);

    qDebug() << ob2.field1;

    //
    channel = new ModbusChannel();

    //wrie and read

    channel->commit(ModbusDriverAddress(HEADER),QVariant(0xabcd));
    QVariant value2(0x0000);
    channel->update(ModbusDriverAddress(HEADER),value2);
    qDebug() << value2.toString();

    //
    theObject ob4={1,2,3};
    QVariant value4;
    value4.setValue(ob4);
    channel->commit(ModbusDriverAddress(BODY),value4);
    ob4 = {4,5,6};
    value4.setValue(ob4);
    channel->update(ModbusDriverAddress(BODY),value4);
    qDebug() << value4.toString();
}
