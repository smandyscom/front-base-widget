#ifndef SERIALIZEDCLIENTTEST_H
#define SERIALIZEDCLIENTTEST_H

#include <task.h>
#include <modbussegment.h>
#include <modbusserializedclient.h>
#include <QModbusTcpClient>
#include <QTimer>

class SerializedClientTest : public Task
{
public:
    SerializedClientTest();
public slots:
    virtual void run();
protected slots:

    void onDriverConnected(QModbusDevice::State);

    void onReadRequestUpdated(QModbusDataUnit replyUnit);
    void onWriteRequestUpdated();

protected:
    QModbusTcpClient* driver;

    ModbusSegment* writeRequest;
    ModbusSegment* readRequest;

    ModbusSerializedClient* client;


};

#endif // SERIALIZEDCLIENTTEST_H
