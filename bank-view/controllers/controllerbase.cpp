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
    //! TODO , return when not cared client

    //! could be overriden by derived
    if (!m_isInitialized)
    {
        onInitializing(ack);
        return; // first time received ack
    }

    //! Raising property updating (string copy , performance issue?
    foreach (QVariant var, m_monitor_propertyKeys) {

		//to receivers
        foreach (QObject* receiver, m_receivers) {
            receiver->setProperty(var.toString().toStdString().c_str(),m_monitor_propertyValues(var));
            receiver->setProperty(QString::number(var.toInt()).toStdString().c_str(),m_monitor_propertyValues(var));
        }

		////to myself
		//setProperty(var.toString().toStdString().c_str(), m_monitor_propertyValues(var));
		//setProperty(QString::number(var.toInt()).toStdString().c_str(), m_monitor_propertyValues(var));
    }
}



MODBUS_U_WORD* ControllerBase::registerWatchList(ADDRESS_MODE unoffsetedAddress,QVariant form, QStateMachine* machine,bool isMachineWatchOnly)
{

	//! Clean input address
	ADDRESS_MODE offsettedAddress = toAddressMode(ADDRESS_REGISTER(unoffsetedAddress));
	m_channel->RegisterRoutines(offsettedAddress,form,m_interval,machine,isMachineWatchOnly);
   
    return m_channel->Handle(offsettedAddress);
}
/// Write in
QVariant ControllerBase::m_monitor_propertyValues(QVariant key)
{
    return m_monitor->Value(key.toUInt());
}
//!
//! \brief ControllerBase::m_operator_propertyChanged
//! \param key
//! \param value
//! Write in
void ControllerBase::m_operator_propertyChanged(QVariant key, QVariant value)
{
    m_channel->Access(toAddressMode(key.value<ADDRESS_MODE>()),value);
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

void ControllerBase::AttachReceiver(QObject *receiver)
{
    m_receivers.append(receiver);
}
