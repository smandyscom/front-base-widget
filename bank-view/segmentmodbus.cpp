#include "segmentmodbus.h"
#include <QModbusReply>

SegmentModbus::SegmentModbus(AccessMethod method,
                             QModbusDataUnit request,
                             QObject *parent) : QObject(parent)
{
    this->method = method;
    this->requestUnit = request;
}

void SegmentModbus::replyfinished()
{
     QModbusReply* reply=  qobject_cast<QModbusReply*>(sender());
     emit update(reply->result());
}


SegmentModbus::AccessMethod SegmentModbus::getMethod()
{
    return method;
}

QModbusDataUnit SegmentModbus::getRequest()
{
    return requestUnit;
}
