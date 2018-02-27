#include "modbuschanneltest.h"
#include <QDebug>

ModbusChannelTest::ModbusChannelTest()
{
    //! driver initialize
    //!
    //!
    driver = new QModbusTcpClient();
    QUrl url = QUrl::fromUserInput("127.0.0.1:502");
    driver->setConnectionParameter(QModbusTcpClient::NetworkPortParameter,url.port());
    driver->setConnectionParameter(QModbusTcpClient::NetworkAddressParameter,url.host());
    driver->setTimeout(1000);
    driver->setNumberOfRetries(3);
    bool result = driver->connectDevice();
    qDebug() << QString("Driver connection:%1").arg(result);
    qDebug() << QString("Driver state:%1").arg(QVariant::fromValue(QModbusDevice::State(driver->state())).value<QString>());
    //! connected
    connect(driver,&QModbusTcpClient::stateChanged,this,&ModbusChannelTest::onDriverConnected);



    //channel = new ModbusChannel();

    QVariant value;

    //qRegisterMetaType<theObject>();
    //int id = qMetaTypeId<theObject>();

    theObject ob = {1,2};
    size_t sz0 = sizeof(theObject);
    size_t sz555 = sizeof(alignmentObject);
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
    qDebug() << sizeof(MODBUS_WORD);
    qDebug() << sizeof(MODBUS_LONG);

    size_t sz = utilities::sizeOf(value);

    //qDebug() << ob2.field1;
    //
    client = new ModbusSerializedClient(driver);
    const ModbusSerializedClient* list[] = {client};
    const int values[] = {1,3,4,5};
    int counts = sizeof(list);
    counts = sizeof(values);
    int sz6 = sizeof(ModbusSerializedClient*);
    channel = new ModbusChannel(list,sizeof(list)/sizeof(ModbusSerializedClient*));

    //wrie and read

//    channel->commit(ModbusDriverAddress(HEADER),QVariant(0xabcd));
//    QVariant value2(0x0000);
//    channel->update(ModbusDriverAddress(HEADER),value2);
//    qDebug() << value2.toString();

//    //
//    theObject ob4={1,2,3};
//    QVariant value4;
//    value4.setValue(ob4);
//    channel->commit(ModbusDriverAddress(BODY),value4);
//    ob4 = {4,5,6};
//    value4.setValue(ob4);
//    channel->update(ModbusDriverAddress(BODY),value4);
//    qDebug() << value4.toString();

    //
    cachedValue.setValue(ob2);
    bool b = true;
    quint16 h = 16;
    cachedBool.setValue(b);
    size_t szzz = utilities::sizeOf(cachedBool);
    //cachedBool.setValue(h);
    //szzz= utilities::sizeOf(cachedBool);
}

void ModbusChannelTest::run()
{

}

void ModbusChannelTest::onDriverConnected(QModbusDevice::State state)
{
    qDebug() << QString("Driver state:%1").arg(QVariant::fromValue(state).value<QString>());
    if(driver->state() != QModbusDevice::ConnectedState)
        return;


    //
    connect(channel,&ModbusChannel::raiseUpdateEvent,this,&ModbusChannelTest::onUpdated);

    //!
    //channel->beginUpdate(ModbusDriverAddress(BODY),cachedValue);
    channel->beginUpdate(ModbusDriverAddress(HEADER),cachedBool);
}

void ModbusChannelTest::onUpdated(UpdateEvent* event)
{
    if(event->address == BODY){
        channel->update(ModbusDriverAddress(BODY),cachedValue);
        channel->commit(ModbusDriverAddress(LOOP),cachedValue);
        channel->beginUpdate(ModbusDriverAddress(BODY),cachedValue);

        //test cached bool
    }
    if(event->address == (HEADER & 0x0000ffff)){
        channel->update(ModbusDriverAddress(HEADER),cachedBool);
        //cachedBool.setValue(!cachedBool.value<bool>());
        qDebug() << cachedBool.toString();
        channel->commit(ModbusDriverAddress(LOOP_HEADER),cachedBool);
        channel->beginUpdate(ModbusDriverAddress(HEADER),cachedBool);
    }
}

