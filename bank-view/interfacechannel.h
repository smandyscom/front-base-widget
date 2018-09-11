#ifndef INTERFACECHANNEL_H
#define INTERFACECHANNEL_H

#include <QObject>
#include <QTimer>
#include <QStateMachine>
#include <QApplication>
#include <definitionsbaselayer.h>
#include <utilities.h>

#include <interfacerequest.h>
#include <interfaceclient.h>

#define CLIENT_MAX_NUM 8

using namespace BaseLayer;
//!
//! \brief The InterfaceChannel class
//! The interface defined for Controller/Fronts
//! Bool type would be resolved here
class InterfaceChannel : public QObject
{
    Q_OBJECT
public:
    static InterfaceChannel* Instance()
    {
        if(__instance == nullptr)
            __instance = new InterfaceChannel(qApp);
        return __instance;
    }


    void Clients(QList<InterfaceClient*> clients)
    {
        //! link clients
        for(int i=0;i<clients.count();i++)
        {
            __clients.append(clients[i]);
            connect(__clients[i],&InterfaceClient::requestAcknowledged,this,&InterfaceChannel::onAcknowledged);
        }
    }

    //!
    //! \brief RoutineAccess
    //! \param address
    //! \param dataFrom
    //! Register and trigger the routine access
    void RegisterRoutines(ADDRESS_MODE address,const QVariant dataFrom,int interval = 50);


    MODBUS_U_WORD* Handle(ADDRESS_MODE address)
    {
        return &(__clients[ADDRESS_CLIENT_ID(address)]->Cache()[ADDRESS_REGISTER(address)]);
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
signals:
    void ackownledged(InterfaceRequest ack);
protected slots:
    void onAcknowledged(InterfaceRequest ack);
protected:
    explicit InterfaceChannel(QObject *parent = nullptr);

    QList<InterfaceClient*> __clients;
    QList<QStateMachine*> __stateMachines;
    QMap<ADDRESS_MODE,int> __routines;

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
    //! Accessing interface
    //! Raising asynchrous updating operation to remote
    void __remoteUpdate(ADDRESS_MODE address, QVariant dataForm);




    //!Singleton
    static InterfaceChannel* __instance;


};

#endif // INTERFACECHANNEL_H
