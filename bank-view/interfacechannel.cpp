#include "interfacechannel.h"

InterfaceChannel::InterfaceChannel(QList<InterfaceClient*> clients,QObject *parent) : QObject(parent)
{
//    //! allocate caches
//    for(int i=0;i<CLIENT_MAX_NUM;i++)
//    {
//        quint16* ____cache = new quint16[USHRT_MAX];
//        memset(____cache,0,USHRT_MAX);
//        __cache.append(____cache);
//    }
    //! link clients
    for(int i=0;i<clients.count();i++)
    {
        __clients.append(clients[i]);
        connect(clients[i],&InterfaceClient::requestAcknowledged,this,&InterfaceChannel::onAcknowledged);
    }
}

//!
//! \brief InterfaceChannel::beginAccess
//! \param address
//! \param dataForm
//! Turns
void InterfaceChannel::beginAccess(ADDRESS_MODE address, QVariant dataForm)
{
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
        __update(address,temp); //get current value
        //bitwise manipulation
        if(value.toBool())
            value.setValue(value.value<quint16>() | ADDRESS_BIT_ACCESSOR(address));
        else
            value.setValue(value.value<quint16>() & (~ADDRESS_BIT_ACCESSOR(address)));
        break;
    }
    default:
        break;
    }

    //! write via interface , put specific type of package into
    __clients[ADDRESS_CLIENT_ID(address)]->pushRequest(InterfaceRequest(InterfaceRequest::WRITE,
                                                                      address,
                                                                      value));
}

//!
//! \brief InterfaceChannel::__update
//! \param address
//! \param out
//! Directly return cached data
void InterfaceChannel::__update(uint address, QVariant &out)
{
    quint16* __baseAddress = &__clients[ADDRESS_CLIENT_ID(address)]->Cache()[ADDRESS_REGISTER(address)];

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
    //! Dispatch update event to statemachines
    foreach (QStateMachine* var, __stateMachines) {
        var->postEvent(new UpdateEvent(ack.Address(),ack.Data()));
    }

    //! recycle polling
    if (__routines.contains(ack.Address()))
    {
        //! once this address had been registered as routine , query interval
        QTimer::singleShot(__routines[ack.Address()],this,[ack,this](){
            this->beginAccess(ack.Address(),ack.Data());
        });
    }
}

void InterfaceChannel::RoutineAccess(uint address, const QVariant dataFrom, int interval)
{
    //! registration
    __routines[address] = interval;

    //! first shot
    beginAccess(address,dataFrom);
}

//quint16* InterfaceChannel::__resolveCache(ADDRESS_MODE address)
//{
//    quint8  __clientId = __resolveClient(address);
//    quint16 __registor = __resolveRegister(address);

//    return &__clients[__clientId]->Cache()[];
//}
//uint InterfaceChannel::__resolveBitAccessor(ADDRESS_MODE address)
//{
//    return 0x0001 << reinterpret_cast<quint8*>(&address)[ADDRESS_BIT_ACCESSOR];
//}

//int InterfaceChannel::__resolveClient(uint address)
//{
//    return reinterpret_cast<quint8*>(&address)[ADDRESS_CLIENT_ID];
//}

//quint16 InterfaceChannel::__resolveRegister(uint address)
//{
//    return reinterpret_cast<quint16*>(&address)[ADDRESS_REGISTER];
//}
