#ifndef INTERFACECHANNEL_H
#define INTERFACECHANNEL_H

#include <QObject>
#include <QTimer>
#include <QStateMachine>

#include <definitionsbaselayer.h>
#include <utilities.h>

#include <interfacerequest.h>
#include <interfaceclient.h>

#define CLIENT_MAX_NUM 8

using namespace BaseLayer;
//!
//! \brief The InterfaceChannel class
//! The interface defined for Controller/Fronts
class InterfaceChannel : public QObject
{
    Q_OBJECT
public:
    explicit InterfaceChannel(QList<InterfaceClient*> clients,QObject *parent = nullptr);

    //!
    //! \brief RoutineAccess
    //! \param address
    //! \param dataFrom
    //! Register and trigger the routine access
    void RoutineAccess(ADDRESS_MODE address,const QVariant dataFrom,int interval);

    //! Accessing interface
    //! Raising asynchrous updating operation to remote
    void beginAccess(ADDRESS_MODE address, QVariant dataForm); //

    template<typename T>
    void beginAccess(ADDRESS_MODE address)
    {
        QVariant form = QVariant::fromValue(T());
        beginAccess(address,form);
    }

    //!
    //!
    //! Read from internal buffer
    template<typename T>
    T Access(const ADDRESS_MODE address)
    {
        QVariant result = QVariant::fromValue(T());
        __update(address,result);
        return result.value<T>();
    }

    //!
    //! \brief Access
    //! \param address
    //! \param value
    //! Write operation to remote , update internal buffer as well
    void Access(ADDRESS_MODE address,const QVariant value)
    {
        __commit(address,value);
    }
    //!
    //!
    //!
    template<typename T>
    void Access(ADDRESS_MODE address,T value)
    {
        __commit(address,QVariant::fromValue(value));
    }

//    //!
//    //! \brief EventSocket
//    //! \return
//    //! Generate a event entity for stateMachines
//    //! since stateMachine would consume a entity after processed
//    //! each stateMachine need to connect to this socket
//    UpdateEvent* EventSocket()
//    {
//        return new UpdateEvent(__cachedReplyAddress,__cachedReplyValue);
//    }
//    ADDRESS_MODE CachedReplyAddress() const
//    {
//        return __cachedReplyAddress;
//    }
//    QVariant CachedReplyValue() const
//    {
//        return __cachedReplyValue;
//    }
protected slots:
    void onAcknowledged(InterfaceRequest ack);
protected:
    ADDRESS_MODE __cachedReplyAddress;
    QVariant __cachedReplyValue;

    //!
    //! \brief __commit
    //! \param address
    //! \param value
    //! Write to remote , and internal buffer
    void __commit(ADDRESS_MODE address,QVariant value);
    //!
    //! \brief __update
    //! \param address
    //! \param out
    //! Read value from internal buffer
    void __update(ADDRESS_MODE address,QVariant& out);
//    //!
//    //! \brief __resolveAddress
//    //! \return
//    //! Turns ADDRESS_MODE into handle
//    quint16 *__resolveCache(ADDRESS_MODE address);
//    //!
//    //! \brief __resolveBitAccessor
//    //! \param address
//    //! \return
//    //! Return bit mask
//    uint __resolveBitAccessor(ADDRESS_MODE address);
//    int __resolveClient(ADDRESS_MODE address);
//    int __resolveRegister(ADDRESS_MODE address);

    //QList<quint16*> __cache;
    QList<InterfaceClient*> __clients;
    QList<QStateMachine*> __stateMachines;
    QMap<ADDRESS_MODE,int> __routines;
};

#endif // INTERFACECHANNEL_H
