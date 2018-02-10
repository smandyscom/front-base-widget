#include "serializedclienttest.h"
#include <QUrl>
#include <QDebug>
SerializedClientTest::SerializedClientTest()
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
    connect(driver,&QModbusTcpClient::stateChanged,this,&SerializedClientTest::onDriverConnected);
}
void SerializedClientTest::run()
{

}

void SerializedClientTest::onDriverConnected(QModbusDevice::State state)
{
    qDebug() << QString("Driver state:%1").arg(QVariant::fromValue(state).value<QString>());
    if(driver->state() != QModbusDevice::ConnectedState)
        return;

    connect(driver->sendReadRequest(QModbusDataUnit(QModbusDataUnit::HoldingRegisters,1,1),1),
            &QModbusReply::finished,
            this,
            [](){qDebug()<<"Read done";});

    //! Request initialize
    //!
    //!
    writeRequest = new ModbusSegment(ModbusSegment::WRITE,
                                     QModbusDataUnit(QModbusDataUnit::HoldingRegisters,0,1));
    readRequest = new ModbusSegment(ModbusSegment::READ,
                                    QModbusDataUnit(QModbusDataUnit::HoldingRegisters,1,1));

    //! client initialize
    client = new ModbusSerializedClient(driver);
    //! Fire
    client->pushRequest(readRequest);
}


//!
//! \brief SerializedClientTest::onReadRequestUpdated
//! \param replyUnit
//!
void SerializedClientTest::onReadRequestUpdated(QModbusDataUnit replyUnit)
{
   //! Readout value then feedback
   writeRequest->requestUnit.setValue(0,replyUnit.value(0));
   client->pushRequest(writeRequest);
   qDebug() << QString("Read:%1").arg(replyUnit.value(0));
}

//!
//! \brief SerializedClientTest::onWriteRequestUpdated
//!
void SerializedClientTest::onWriteRequestUpdated()
{
   client->pushRequest(readRequest);
}
