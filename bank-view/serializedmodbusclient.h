#ifndef SERIALIZEDMODBUSCLIENT_H
#define SERIALIZEDMODBUSCLIENT_H

#include <QObject>
#include <QModbusClient>
#include <QQueue>
#include <segmentmodbus.h>

#include <QTimer>


//guarantee the request be handled in some sort of sequence
class SerializedModbusClient : public QObject
{
    Q_OBJECT
public:
    explicit SerializedModbusClient(QObject *parent = nullptr);
    void pushRequest(SegmentModbus *request);

signals:

public slots:

protected slots:
    void popRequest();
protected:
    QQueue<SegmentModbus*> queue;

    int serverAddress;
    QModbusClient* gateway; //should be initialed somewhere

    QTimer* timer;
};

#endif // SEQUENTIALMODBUSCLIENT_H
