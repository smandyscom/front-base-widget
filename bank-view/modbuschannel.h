#ifndef MODBUSCHANNEL_H
#define MODBUSCHANNEL_H

#include <QObject>
#include <QList>
#include <QPair>
#include <modbuscluster.h>
#include <QMap>
#include <modbusserializedclient.h>

#include <baselayerdefinitions.h>
using namespace BaseLayer;

typedef QPair<ModbusCluster::Attribute,size_t> ModbusClusterConfiguration;



//!
//! \brief The ModbusChannel class
//! event source?
//! composite clusters
class ModbusChannel : public QObject
{
    Q_OBJECT
public:
    explicit ModbusChannel(QModbusClient* driver=nullptr,
                           int serverAddress=1,
                           QObject *parent = nullptr);

    //!1
    //! Accessing interface
    void beginReadData(ModbusDriverAddress address); //raising asynchrous updating operation
    QVariant readData(ModbusDriverAddress address); // would not raise updating action
    void writeData(ModbusDriverAddress address,QVariant value);

    //!2
    //! Configuration (data map
    template<typename T>  void registerBinding(ModbusDriverAddress address)
    {
        //for those structure type
        int id = qRegisterMetaType<T>();
        T initialValue; //allocate initial value
        dataMap[address] = QVariant::fromValue(initialValue); //take into map
    }
    int registerBinding(QList<QModbusBinding> list); //address,value pair

    //!3
    //! Configuration (clusters
    void configureClusters(QList<ModbusClusterConfiguration> configrationList);

    //!4
    //! Tool function
    int queryCluster(ModbusDriverAddress address); //return cluster id
    bool isInCluster(ModbusDriverAddress address,int clusterId){return queryCluster(address) == clusterId;}
signals:
    void clusterUpdated(int clusterId);
    //!
    //! \brief updated controller should transform this signal into event if state machine is avialable
    //! \param address
    //! \param value
    //!
    void updated(AbstractAddress address,QVariant value);

public slots:

    //! Source : cluster
    //! \brief clusterUpdated
    void clusterUpdated();
    //! Source : cluster
    //! \brief requestRaised
    //! \param request
    //!
 protected slots:
    void requestRaised(const ModbusSegment* request); //bridge cluster and gateway
protected:

    quint16* toStartAddress(ModbusDriverAddress address);

    QMap<ModbusDriverAddress,QVariant> dataMap; // address,value

    QList<ModbusCluster*> clusterCollection; //index as cluster id
    quint16 memory[UINT16_MAX]; //65536

    ModbusSerializedClient* requestGateWay;
};

#endif // CHANNEL_H
