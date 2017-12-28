#ifndef MODBUSSERIALIZEDCLIENT_H
#define MODBUSSERIALIZEDCLIENT_H

#include <QObject>
#include <QModbusClient>
#include <QQueue>
#include <modbussegment.h>

#include <QTimer>


//guarantee the request be handled in some sort of sequence
class ModbusSerializedClient : public QObject
{
    Q_OBJECT
public:
    explicit ModbusSerializedClient(QModbusClient* driver = nullptr,QObject *parent = nullptr);
    void pushRequest(ModbusSegment *request);

    //!1 Configuration
    //!
    void setServerAddress(int serverAddress){this->serverAddress = serverAddress;}
    void setDriver(QModbusClient* driver){this->driver=driver;}

signals:

public slots:

protected slots:
    void popRequest();
protected:

    QQueue<ModbusSegment*> requestQueue;

    int serverAddress;
    QModbusClient* driver; //should be initialed somewhere

    QTimer* timer; //driving consumer
};

#endif // SEQUENTIALMODBUSCLIENT_H
