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
    explicit ModbusChannel(QObject *parent = nullptr);

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


//    //!2
//    //! Configuration (data map
//    template<typename T>  void registerBinding(ModbusDriverAddress address)
//    {
//        //for those structure type
//        int id = qRegisterMetaType<T>();
//        T initialValue; //allocate initial value
//        dataMap[address] = QVariant::fromValue(initialValue); //take into map
//    }
//    int registerBinding(QList<QModbusBinding> list); //address,value pair

signals:
    void raiseUpdateEvent(UpdateEvent* event);
public slots:

 protected slots:
    //void requestRaised(const ModbusSegment* request); //bridge cluster and gateway
    //!
    //! \brief onReplyUpdated
    //! \param result
    //! Reply processor
    void onUpdated(QModbusDataUnit result);
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
};

#endif // CHANNEL_H
