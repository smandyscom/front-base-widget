#include "controllerbase.h"
#include <QDebug>

ControllerBase::ControllerBase(quint8 clientId, quint16 baseOffset, int interval, QObject *parent) :
    QObject(parent),
    m_clientId(clientId),
    m_baseOffset(baseOffset),
    m_interval(interval)
{
    m_isInitialized = false;
    m_channel = InterfaceChannel::Instance();

    connect(m_channel,&InterfaceChannel::ackownledged,this,&ControllerBase::onAcknowledged);
}

ADDRESS_MODE ControllerBase::toAddressMode(ADDRESS_MODE unoffseted) const
{
    return (unoffseted & 0x00ffffff) +
            m_clientId * 0x01000000 +
            m_baseOffset;
}

//!
//! \brief ControllerBase::onAcknowledged
//! \param ack
//! Pumping dynamic property values to parent (Routine update
void ControllerBase::onAcknowledged(InterfaceRequest ack)
{
    //! could be overriden by derived
    if (!m_isInitialized)
    {
        onInitializing(ack);
        return; // first time received ack
    }

    //! Raising property updating (string copy , performance issue?
    foreach (QVariant var, m_monitor_propertyKeys) {
       parent()->setProperty(var.toStdString().c_str(),m_monitor_propertyValues(var));
       parent()->setProperty(var.toInt().c_str(),m_monitor_propertyValues(var));
    }
}



MODBUS_U_WORD* ControllerBase::registerWatchList(ADDRESS_MODE unoffsetedAddress,QVariant form)
{
    ADDRESS_MODE offsettedAddress = toAddressMode(unoffsetedAddress);
    m_channel->RegisterRoutines(toAddressMode(offsettedAddress),form,m_interval);

    return m_channel->Handle(offsettedAddress);
}

QVariant ControllerBase::m_monitor_propertyValues(QVariant key)
{
    return m_monitor->Value(key.toUInt());
}

bool ControllerBase::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::DynamicPropertyChange:
        //!intercept and handling
        //! Raise when property changed only
    {
        auto value = property(static_cast<QDynamicPropertyChangeEvent*>(event)->propertyName());
        auto key = QString(static_cast<QDynamicPropertyChangeEvent*>(event)->propertyName());
        m_operator_propertyChanged(m_operator_propertyKeys[key],value);
        break;
    }
    default:
        break;
    }

    return QObject::event(event);
}
