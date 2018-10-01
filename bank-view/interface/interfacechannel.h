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
        if(m_instance == nullptr)
            m_instance = new InterfaceChannel(qApp);
        return m_instance;
    }


    void Clients(QList<InterfaceClient*> clients)
    {
        //! link clients
        for(int i=0;i<clients.count();i++)
        {
            m_clients.append(clients[i]);
            connect(m_clients[i],&InterfaceClient::requestAcknowledged,this,&InterfaceChannel::onAcknowledged);
        }
    }

    //!
    //! \brief RoutineAccess
    //! \param address
    //! \param dataFrom
    //! Register and trigger the routine access
    void RegisterRoutines(ADDRESS_MODE address,const QVariant dataFrom,int interval = 50);
    QMap<ADDRESS_MODE,int> Routines() const;

    void RegisterStateMachine(QStateMachine* machine);

    MODBUS_U_WORD* Handle(ADDRESS_MODE address)
    {
        return &(m_clients[ADDRESS_CLIENT_ID(address)]->Cache()[ADDRESS_REGISTER(address)]);
    }

    //! Read from internal buffer
//    template<typename T>
//    T Update(const ADDRESS_MODE address)
//    {
//        QVariant result = QVariant::fromValue(T());
//        m_update(address,result);
//        return result.value<T>();
//    }
    //!
    //! \brief BeginRead
    //! \param address
    //! \param dataForm
    //!
    void BeginRead(ADDRESS_MODE address,QVariant dataForm)
    {
        m_remoteUpdate(address,dataForm);
    }

    //!
    //! \brief Access
    //! \param address
    //! \param value
    //! Write operation to remote , update internal buffer as well
    void Access(ADDRESS_MODE address,const QVariant value)
    {
        m_commit(address,value);
    }

signals:
    void ackownledged(InterfaceRequest ack);
protected slots:
    void onAcknowledged(InterfaceRequest ack);
protected:
    explicit InterfaceChannel(QObject *parent = nullptr);

    QList<InterfaceClient*> m_clients;
    QList<QStateMachine*> m_stateMachines;
    QMap<ADDRESS_MODE,int> m_routines;

    //!
    //! \brief __commit
    //! \param address
    //! \param value
    //! Write to remote , and internal buffer
    void m_commit(ADDRESS_MODE address,QVariant value);
    //!
    //! \brief __update
    //! \param address
    //! \param out
    //! Read value from internal buffer
    void m_update(ADDRESS_MODE address,QVariant& out);
    //! Accessing interface
    //! Raising asynchrous updating operation to remote
    void m_remoteUpdate(ADDRESS_MODE address, QVariant dataForm);




    //!Singleton
    static InterfaceChannel* m_instance;


};

#endif // INTERFACECHANNEL_H