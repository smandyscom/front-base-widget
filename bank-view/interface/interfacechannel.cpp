#include "interfacechannel.h"
#include <QDebug>
InterfaceChannel::InterfaceChannel(QObject *parent) : QObject(parent)
{

}

//!
//! \brief InterfaceChannel::beginAccess
//! \param address
//! \param dataForm
//! Turns
void InterfaceChannel::m_remoteUpdate(ADDRESS_MODE address, QVariant dataForm)
{
    //! For bool type (bit access) , update whole word
    if(dataForm.type() == QVariant::Bool)
        dataForm = QVariant::fromValue(static_cast<quint16>(0));

    if(ADDRESS_CLIENT_ID(address) >= m_clients.count())
    {
        qDebug() << QString("interface client %1 not found").arg(ADDRESS_CLIENT_ID(address));
        return;
    }

    //! read via interface , put specific type of package into
    m_clients[ADDRESS_CLIENT_ID(address)]->pushRequest(InterfaceRequest(InterfaceRequest::READ,
                                                                      address,
                                                                      dataForm));
}
//!
//! \brief InterfaceChannel::m_commit
//! \param address
//! \param value
//! for series bool setting , need to be pre-updated?
void InterfaceChannel::m_commit(ADDRESS_MODE address, QVariant value)
{
    QVariant temp = value;

    switch (value.type()) {
    case QVariant::Bool:
    {
//        temp = QVariant::fromValue(static_cast<quint16>(0));
//        m_update(address,temp); //get current value
        quint16* previousValue = Handle(address);
        quint16 bitAccessor = ADDRESS_BIT_ACCESSOR(address);
        //bitwise manipulation
        if(value.toBool())
            *previousValue |= bitAccessor;
        else
            *previousValue &= ~bitAccessor;

        temp = QVariant::fromValue(*previousValue);
        break;
    }
    default:
        break;
    }

    //! write via interface , put specific type of package into
    m_clients[ADDRESS_CLIENT_ID(address)]->pushRequest(InterfaceRequest(InterfaceRequest::WRITE,
                                                                      address,
                                                                      temp));
}

//!
//! \brief InterfaceChannel::__update
//! \param address
//! \param out
//! Directly return cached data
void InterfaceChannel::m_update(ADDRESS_MODE address, QVariant &out)
{
    quint16* baseAddress = Handle(address);

    switch (out.type()) {
    case QVariant::Bool:
    {
        if(*baseAddress & ADDRESS_BIT_ACCESSOR(address))
            out.setValue(true);
        else
            out.setValue(false);
        break;
    }
    default:
        //!Pack to QVariant
        memcpy(out.data(),
               baseAddress,
               utilities::sizeOf(out)) ;
        break;
    }
}

//!
//! \brief InterfaceChannel::onAckowledged
//! \param ack
//! Acknowledged from source
void InterfaceChannel::onAcknowledged(InterfaceRequest ack)
{
    const QVariant data = ack.Data();
	uint ackAddress = ack.Address();
    //! Dispatch update event to statemachines
    foreach (ADDRESS_MODE var, m_stateMachines.keys()) {
		if (ADDRESS_CLIENT_ID(ackAddress) == ADDRESS_CLIENT_ID(var) &&
			ADDRESS_REGISTER(ackAddress) == ADDRESS_REGISTER(var))
		{
			//intrested address
			for each (QStateMachine* machine in m_stateMachines.values())
			{
				machine->postEvent(new UpdateEvent(ack.Address(),data));
			}
		}
    }

    //! recycle polling
    if (m_routines.contains(ack.Address()))
    {
        //! once this address had been registered as routine , query interval
        QTimer::singleShot(m_routines[ack.Address()],this,[ack,this,data](){
            this->m_remoteUpdate(ack.Address(),data);
        });
    }
    //! Dispatch signal to upper
    emit ackownledged(ack);
}

void InterfaceChannel::RegisterRoutines(ADDRESS_MODE address, const QVariant dataFrom, int interval,QStateMachine* machine, bool isMachineWatchOnly)
{
    if(ADDRESS_CLIENT_ID(address) >= m_clients.count())
    {
        qDebug() << QString("client id %1 , not found,").arg(ADDRESS_CLIENT_ID(address));
        return;
    }

    //! registration
	if(!isMachineWatchOnly)
	{
		m_routines[address] = interval;
		//! first shot
		m_remoteUpdate(address,dataFrom);
	}
		
	if (machine != nullptr)
	{
		m_stateMachines[address] = machine;
	}

}

QMap<ADDRESS_MODE,int> InterfaceChannel::Routines() const
{
    return m_routines;
}

//void InterfaceChannel::RegisterStateMachine(QStateMachine *machine)
//{
//    m_stateMachines.append(machine);
//}

bool InterfaceChannel::IsAllConnected() const
{
	bool result = true;
	for each (InterfaceClient* var in m_clients)
	{
		result &= var->IsConnected();
	}
	return result;
}

InterfaceChannel* InterfaceChannel::m_instance = nullptr;
