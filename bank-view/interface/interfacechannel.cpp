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

    //! read via interface , put specific type of package into
    m_clients[ADDRESS_CLIENT_ID(address)]->pushRequest(InterfaceRequest(InterfaceRequest::READ,
                                                                      address,
                                                                      dataForm));
}

void InterfaceChannel::m_commit(ADDRESS_MODE address, QVariant value)
{
    QVariant temp = value;

    switch (value.type()) {
    case QVariant::Bool:
    {
        temp = QVariant::fromValue(static_cast<quint16>(0));
        m_update(address,temp); //get current value
        quint16 tempValue = 0;
        quint16 bitAccessor = ADDRESS_BIT_ACCESSOR(address);
        //bitwise manipulation
        if(value.toBool())
            tempValue = temp.value<quint16>() | bitAccessor;
        else
            tempValue = temp.value<quint16>() & ~bitAccessor;

        temp.setValue(tempValue);
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
    quint16* __baseAddress = Handle(address);

    switch (out.type()) {
    case QVariant::Bool:
    {
        if(*__baseAddress & ADDRESS_BIT_ACCESSOR(address))
            out.setValue(true);
        else
            out.setValue(false);
        break;
    }
    default:
        //!Pack to QVariant
        memcpy(out.data(),
               __baseAddress,
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
    const QVariant __data = ack.Data();

    //! Dispatch update event to statemachines
    foreach (QStateMachine* var, m_stateMachines) {
        var->postEvent(new UpdateEvent(ack.Address(),__data));
    }

    //! recycle polling
    if (m_routines.contains(ack.Address()))
    {
        //! once this address had been registered as routine , query interval
        QTimer::singleShot(m_routines[ack.Address()],this,[ack,this,__data](){
            this->m_remoteUpdate(ack.Address(),__data);
        });
    }
    //! Dispatch signal to upper
    emit ackownledged(ack);
}

void InterfaceChannel::RegisterRoutines(ADDRESS_MODE address, const QVariant dataFrom, int interval)
{
    if(ADDRESS_CLIENT_ID(address) >= m_clients.count())
    {
        qDebug() << QString("client id %1 , not found,").arg(ADDRESS_CLIENT_ID(address));
        return;
    }

    //! registration
    m_routines[address] = interval;

    //! first shot
    m_remoteUpdate(address,dataFrom);
}

QMap<ADDRESS_MODE,int> InterfaceChannel::Routines() const
{
    return m_routines;
}

void InterfaceChannel::RegisterStateMachine(QStateMachine *machine)
{
    m_stateMachines.append(machine);
}

InterfaceChannel* InterfaceChannel::m_instance = nullptr;
