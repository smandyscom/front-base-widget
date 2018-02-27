#ifndef MODBUSCHANNEL_H
#define MODBUSCHANNEL_H

#include <QObject>
#include <QList>
#include <QPair>
#include <QMap>
#include <modbusserializedclient.h>

#include <baselayerdefinitions.h>
using namespace BaseLayer;

#define CHANNEL_NUM 16

//!
//! \brief The ModbusChannel class
//! event source?
//! composite clusters
//! (*Going to be generalized as common HAL Layer
class ModbusChannel : public QObject
{


    Q_OBJECT
public:
    explicit ModbusChannel(const ModbusSerializedClient* clientList[],
                           const int channelCounts,
                           QObject *parent = nullptr);

    //!1
    //! Accessing interface
    //! raising asynchrous updating operation
    void beginUpdate(ModbusDriverAddress address,const QVariant dataForm); //
    //!
    //! \brief readData
    //! \param address
    //! \return
    //! synchrous function , direct return cached value
    void update(const ModbusDriverAddress modbusAddress,QVariant& fetchOut);
    //!
    //! \brief writeData
    //! \param address
    //! \param value
    //! Fire and forget
    void commit(ModbusDriverAddress address, const QVariant value);


    //!
    //! \brief getInstance
    //! \return
    //! Singleton
    //static ModbusChannel* getInstance();

signals:
    //!
    //! \brief raiseUpdateEvent
    //! \param event
    //! The event source ready to connect with QStateMachine
    void raiseUpdateEvent(UpdateEvent* event);

protected slots:
    //void requestRaised(const ModbusSegment* request); //bridge cluster and gateway
    //!
    //! \brief onReplyUpdated
    //! \param result
    //! Reply processor
    void onRequestProcessed(QModbusDataUnit result);

protected:
    //!
    //! \brief dataMap
    //! Build Address-Value pair , type info included in QVariant
    //QMap<ModbusDriverAddress,QVariant> dataMap;



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
private:
    static ModbusChannel* instance;

};

#endif // CHANNEL_H
