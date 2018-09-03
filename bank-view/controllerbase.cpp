#include "controllerbase.h"

ControllerBase::ControllerBase(quint8 clientId, quint16 baseOffset, int interval, QObject *parent) :
    QObject(parent),
    __clientId(clientId),
    __baseOffset(baseOffset),
    __interval(interval),
    __channel(InterfaceChannel::Instance())
{
    //! register routines
}

ADDRESS_MODE ControllerBase::toAddressMode(ADDRESS_MODE unoffseted)
{
    return (unoffseted & 0x00ffffff) +
            __clientId * 0x01000000 +
            __baseOffset;
}

void ControllerBase::onAcknowledged(InterfaceRequest ack)
{
    //! could be overriden by derived
}
