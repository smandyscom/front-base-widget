#ifndef MODBUSSERIALIZEDCLIENT_H
#define MODBUSSERIALIZEDCLIENT_H

#include <QObject>
#include <QModbusClient>
#include <QQueue>
#include <modbussegment.h>

#include <QTimer>


//guarantee the request be handled in some sort of sequence
//!
//! \brief The ModbusSerializedClient class
//! Make sure requests are processed in sequence
//! Face to certain server address
class ModbusSerializedClient : public QObject
{
    Q_OBJECT
public:
    explicit ModbusSerializedClient(QModbusClient* driver = nullptr,
                                    int serverAddress=1,
                                    QObject *parent = nullptr);
    void pushRequest(const ModbusSegment *request);
signals:

public slots:

protected slots:
    void popRequest();
protected:

    QQueue<const ModbusSegment*> requestQueue;

    int serverAddress;
    QModbusClient* driver; //should be initialed somewhere

    QTimer* timer; //driving consumer
};

#endif // SEQUENTIALMODBUSCLIENT_H
