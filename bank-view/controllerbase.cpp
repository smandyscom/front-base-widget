#include "controllerbase.h"
#include <QDebug>

ControllerBase::ControllerBase(quint8 clientId, quint16 baseOffset, int interval, QObject *parent) :
    QObject(parent),
    __clientId(clientId),
    __baseOffset(baseOffset),
    __interval(interval)
{
    __isInitialized = false;
    __channel = InterfaceChannel::Instance();

    connect(__channel,&InterfaceChannel::ackownledged,this,&ControllerBase::onAcknowledged);
}

ADDRESS_MODE ControllerBase::toAddressMode(ADDRESS_MODE unoffseted) const
{
    return (unoffseted & 0x00ffffff) +
            __clientId * 0x01000000 +
            __baseOffset;
}

void ControllerBase::onAcknowledged(InterfaceRequest ack)
{
    //! could be overriden by derived
    if (!__isInitialized)
    {
        onInitializing(ack);
        return; // first time received ack
    }

    //!if not on watch list , return (performance

    //! Raising property updating (string copy , performance issue?
    foreach (QVariant var, __propertyKeys) {
       parent()->setProperty(var.toString().toStdString().c_str(),propertyValues(var));
    }
}



void ControllerBase::registerWatchList(ADDRESS_MODE unoffsetedAddress,QVariant form)
{
    ADDRESS_MODE __address = toAddressMode(unoffsetedAddress);
    __channel->RegisterRoutines(toAddressMode(__address),form,__interval);
}

QVariant ControllerBase::propertyValues(QVariant key)
{
    return QVariant::fromValue(0);
}
