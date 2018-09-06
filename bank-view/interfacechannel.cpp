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
void InterfaceChannel::__remoteUpdate(ADDRESS_MODE address, QVariant dataForm)
{
    //! For bool type (bit access) , update whole word
    if(dataForm.type() == QVariant::Bool)
        dataForm = QVariant::fromValue(static_cast<quint16>(0));

    //! read via interface , put specific type of package into
    __clients[ADDRESS_CLIENT_ID(address)]->pushRequest(InterfaceRequest(InterfaceRequest::READ,
                                                                      address,
                                                                      dataForm));
}

void InterfaceChannel::__commit(uint address, QVariant value)
{
    QVariant temp = value;

    switch (value.type()) {
    case QVariant::Bool:
    {
        temp = QVariant::fromValue(static_cast<quint16>(0));
        __update(address,temp); //get current value
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
    __clients[ADDRESS_CLIENT_ID(address)]->pushRequest(InterfaceRequest(InterfaceRequest::WRITE,
                                                                      address,
                                                                      temp));
}

//!
//! \brief InterfaceChannel::__update
//! \param address
//! \param out
//! Directly return cached data
void InterfaceChannel::__update(uint address, QVariant &out)
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
    foreach (QStateMachine* var, __stateMachines) {
        var->postEvent(new UpdateEvent(ack.Address(),__data));
    }

    //! recycle polling
    if (__routines.contains(ack.Address()))
    {
        //! once this address had been registered as routine , query interval
        QTimer::singleShot(__routines[ack.Address()],this,[ack,this,__data](){
            this->__remoteUpdate(ack.Address(),__data);
        });
    }
    //! Dispatch signal to upper
    emit ackownledged(ack);
}

void InterfaceChannel::RegisterRoutines(uint address, const QVariant dataFrom, int interval)
{
    //! registration
    __routines[address] = interval;

    //! first shot
    __remoteUpdate(address,dataFrom);
}


InterfaceChannel* InterfaceChannel::__instance = nullptr;
