#ifndef INTERFACECHANNEL_H
#define INTERFACECHANNEL_H

#include <QObject>
#include <definitionsbaselayer.h>

#define ADDRESS_MODE uint

//!
//! \brief The InterfaceChannel class
//! The interface defined for Controller/Fronts
class InterfaceChannel : public QObject
{
    Q_OBJECT
public:
    explicit InterfaceChannel(QObject *parent = nullptr);

    //! Accessing interface
    //! Raising asynchrous updating operation to remote
    void beginAccess(ADDRESS_MODE address,const QVariant dataForm); //

    template<typename T>
    inline
    void beginAccess(ADDRESS_MODE address)
    {
        QVariant form = QVariant::fromValue(T());
        beginAccess(address,form);
    }

    //!
    //!
    //! Read from internal buffer
    template<typename T>
    inline
    T Access(const ADDRESS_MODE address)
    {
        QVariant result = QVariant::fromValue(T());
        this->__update(address,result);
        return result.value<T>();
    }

    //!
    //! \brief Access
    //! \param address
    //! \param value
    //! Write operation to remote
    void Access(ADDRESS_MODE address,const QVariant value)
    {
        __commit(address,value);
    }
    //!
    //!
    //!
    template<typename T>
    inline
    void Access(ADDRESS_MODE address,T value)
    {
        __commit(address,QVariant::fromValue(value));
    }

    //!
    //! \brief getInstance
    //! \return
    //! Singleton
//    static ModbusChannel* Instance();
//    static void Channels(QList<ModbusSerializedClient*> value);
//    static QList<ModbusSerializedClient*> Channels();

    //!
    //! \brief EventSocket
    //! \return
    //! Generate a event entity for stateMachines
    //! since stateMachine would consume a entity after processed
    //! each stateMachine need to connect to this socket
    UpdateEvent* EventSocket()
    {
        return new UpdateEvent(__cachedReplyAddress,__cachedReplyValue);
    }
    ModbusDriverAddress CachedReplyAddress() const
    {
        return __cachedReplyAddress;
    }
    QVariant CachedReplyValue() const
    {
        return __cachedReplyValue;
    }


signals:
    //!
    //! \brief raiseUpdateEvent
    //! \param event
    //! The event source ready to connect with QStateMachine
    void readReply();

public slots:
};

#endif // INTERFACECHANNEL_H
