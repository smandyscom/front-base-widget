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

//!
//! \brief ControllerBase::onAcknowledged
//! \param ack
//! Pumping dynamic property values to parent (Routine update
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



MODBUS_U_WORD* ControllerBase::registerWatchList(ADDRESS_MODE unoffsetedAddress,QVariant form)
{
    ADDRESS_MODE offsettedAddress = toAddressMode(unoffsetedAddress);
    __channel->RegisterRoutines(toAddressMode(offsettedAddress),form,__interval);

    return __channel->Handle(offsettedAddress);
}

QVariant ControllerBase::propertyValues(QVariant key)
{
    return m_monitor->Value(key.toUInt());
}
