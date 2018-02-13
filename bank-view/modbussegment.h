#ifndef MODBUSSEGMENT_H
#define MODBUSSEGMENT_H

#include <QObject>
#include <QModbusDataUnit>


//QObject is forbidden to copy/assign
//!
//! \brief The ModbusSegment class
//!
class ModbusSegment : public QObject
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

    explicit ModbusSegment(AccessMethod method,
                           QModbusDataUnit request,
                           QObject *parent = nullptr);

    ModbusSegment::AccessMethod getMethod() const;
    void setMethod(AccessMethod method);

    QModbusDataUnit requestUnit;
signals:
    void update(QModbusDataUnit replyUnit); //connet QModbusReply.finished
protected:
    AccessMethod method;


};

#endif // SEGMENTMODBUS_H
