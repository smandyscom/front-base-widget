#include "controllerbase.h"
#include <QDebug>

ControllerBase::ControllerBase(quint8 clientId, quint16 baseOffset, int interval, QObject *parent) :
    QObject(parent),
    m_clientId(clientId),
    m_baseOffset(baseOffset),
    m_interval(interval)
{
    //m_isInitialized = false;
    m_channel = InterfaceChannel::Instance();

    connect(m_channel,&InterfaceChannel::ackownledged,this,&ControllerBase::onAcknowledged);

	attach(new PropertyPortCommon(this));

	//!
	m_updateTimer = new QTimer(this);
	connect(m_updateTimer, &QTimer::timeout, this, &ControllerBase::onUpdate);
	m_updateTimer->start(100);
}

ADDRESS_MODE ControllerBase::toAddressMode(ADDRESS_MODE unoffseted) const
{
    return (unoffseted & 0x00ffffff) +
            m_clientId * 0x01000000 +
            m_baseOffset;
}

QObject* ControllerBase::port() const
{
	return m_port;
}
//!
//! \brief ControllerBase::onAcknowledged
//! \param ack
//! Pumping dynamic property values to parent (Routine update
void ControllerBase::onAcknowledged(InterfaceRequest ack)
{
    //! TODO , return when not cared client
	ADDRESS_MODE addr = ack.Address();
	quint8 cid = ADDRESS_CLIENT_ID(addr);

	if (cid != m_clientId)
		return;
}

void ControllerBase::onUpdate()
{
	foreach(QVariant var, m_monitor_propertyKeys) {
			emit m_port->externalPropertyChange(var, m_monitor_propertyValues(var));
			emit m_port->externalPropertyChange(QString::number(var.toInt()), m_monitor_propertyValues(var));
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
void ControllerBase::onPropertyChanged(QVariant key, QVariant value)
{
    m_channel->Access(toAddressMode(key.value<ADDRESS_MODE>()),value);
}

void ControllerBase::attach(QObject *receiver)
{
    m_port= qobject_cast<PropertyPortCommon*>(receiver);

	PropertyPortCommon* ppc = qobject_cast<PropertyPortCommon*>(receiver);
	connect(ppc, &PropertyPortCommon::internalPropertyChange, this, &ControllerBase::onPropertyChanged);
}
