#ifndef SEGMENTMODBUS_H
#define SEGMENTMODBUS_H

#include <QObject>
#include <QModbusDataUnit>


//QObject is forbidden to copy/assign
class SegmentModbus : public QObject
{
    Q_OBJECT
public:
    enum AccessMethod
    {
        READ,
        WRITE,
        READ_WRITE,// reserved
    };
    Q_ENUM(AccessMethod)

    explicit SegmentModbus(AccessMethod method,
                           QModbusDataUnit request,
                           QObject *parent = nullptr);

    AccessMethod getMethod();
    void setMethod(AccessMethod method);
    QModbusDataUnit getRequest();

signals:
    //update target , mark down dirty if RB
    void update(QModbusDataUnit replyUnit); //connet QModbusReply.finished
public slots:
    void replyfinished();

protected:
    AccessMethod method;
    QModbusDataUnit requestUnit;

};

#endif // SEGMENTMODBUS_H
