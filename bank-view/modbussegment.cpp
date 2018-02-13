#include "modbussegment.h"
#include <QModbusReply>

ModbusSegment::ModbusSegment(AccessMethod method,
                             QModbusDataUnit request,
                             QObject *parent) : QObject(parent)
{
    this->method = method;
    this->requestUnit = request;
}

ModbusSegment::AccessMethod ModbusSegment::getMethod() const
{
    return method;
}

void ModbusSegment::setMethod(AccessMethod method)
{
    this->method = method;
}
