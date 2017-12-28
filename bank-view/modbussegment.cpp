#include "modbussegment.h"
#include <QModbusReply>

ModbusSegment::ModbusSegment(AccessMethod method,
                             QModbusDataUnit request,
                             QObject *parent) : QObject(parent)
{
    this->method = method;
    this->requestUnit = request;
}

void ModbusSegment::replyfinished()
{
     QModbusReply* reply=  qobject_cast<QModbusReply*>(sender());
     emit update(reply->result());
}


ModbusSegment::AccessMethod ModbusSegment::getMethod() const
{
    return method;
}

void ModbusSegment::setMethod(AccessMethod method)
{
    this->method = method;
}

QModbusDataUnit ModbusSegment::getRequest() const
{
    return requestUnit;
}
