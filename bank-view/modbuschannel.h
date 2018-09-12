#ifndef MODBUSCHANNEL_H
#define MODBUSCHANNEL_H

#include <QObject>
#include <QList>
#include <QPair>
#include <QMap>
#include <modbusserializedclient.h>

#include <definitionsbaselayer.h>
using namespace BaseLayer;

#define CHANNEL_MAX_NUM 16

//!
//! \brief The ModbusChannel class
//! event source?
//! composite clusters
//! (*Going to be generalized as common HAL Layer
class ModbusChannel : public QObject
{
    Q_OBJECT
public:
    //!1
    //! Accessing interface
    //! raising asynchrous updating operation
    void beginAccess(ModbusDriverAddress address,const QVariant dataForm); //

    template<typename T>
    inline
    void beginAccess(ModbusDriverAddress address)
    {
        QVariant form = QVariant::fromValue(T());
        beginAccess(address,form);
    }
    //!
    //!
    //!Read
    template<typename T>
    inline
    T Access(const ModbusDriverAddress address)
    {
        QVariant result = QVariant::fromValue(T());
        this->__update(address,result);
        return result.value<T>();
    }


    //!Write
    void Access(ModbusDriverAddress address,const QVariant value)
    {
        __commit(address,value);
    }
    template<typename T>
    inline
    void Access(ModbusDriverAddress address,T value)
    {
        __commit(address,QVariant::fromValue(value));
    }

    //!
    //! \brief getInstance
    //! \return
    //! Singleton
    static ModbusChannel* Instance();
    static void Channels(QList<ModbusSerializedClient*> value);
    static QList<ModbusSerializedClient*> Channels();

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

protected slots:
    //!
    //! \brief onReplyUpdated
    //! \param result
    //! Reply processor
    void onReadRequestProcessed(QModbusDataUnit result);

protected:
    //!
    //! \brief readData
    //! \param address
    //! \return
    //! synchrous function , direct return cached value
    void __update(const ModbusDriverAddress modbusAddress,QVariant& fetchOut) const ;
    //!
    //! \brief writeData
    //! \param address
    //! \param value
    //! Fire and forget
    void __commit(ModbusDriverAddress address, const QVariant value);

    QModbusDataUnit preparedReadRequest;
    QModbusDataUnit preparedWriteRequest;

    QList<quint16*> channelCache; //65536 words each channel
    QList<ModbusSerializedClient*> channelGateWays;

    //!
    //! \brief toCacheAddress
    //! \param modbusAddress
    //! \return
    //! internal utility function
    quint16* toCacheAddress(const ModbusDriverAddress modbusAddress);

    //!
    //! \brief writeData
    //! \param modbusAddress
    //! \param source
    //! update both cache and variant
    void writeData(ModbusDriverAddress modbusAddress, const void *source, size_t count);

    explicit ModbusChannel(QList<ModbusSerializedClient*> clientList,
                           QObject *parent = nullptr);

    ModbusDriverAddress __cachedReplyAddress;
    QVariant __cachedReplyValue;

    static ModbusChannel* __instance;
    static QList<ModbusSerializedClient*> __channels;
};

#endif // CHANNEL_H
