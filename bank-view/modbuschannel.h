#ifndef MODBUSCHANNEL_H
#define MODBUSCHANNEL_H

#include <QObject>
#include <QList>
#include <QPair>
#include <modbuscluster.h>
#include <QMap>
#include <modbusserializedclient.h>

#include <baselayerdefinitions.h>

typedef QPair<ModbusCluster::Attribute,size_t> ModbusClusterConfiguration;

//composite clusters
class modbusChannel : public QObject
{
    Q_OBJECT
public:
    explicit modbusChannel(QObject *parent = nullptr);

    //!1
    //! Accessing interface
    void beginReadData(QVariant address); //raising asynchrous updating operation
    QVariant readData(QVariant address); // would not raise updating action
    writeData(QVariant address,QVariant value);

    //!2
    //! Configuration (data map
    template<typename T>  void registerBinding(QVariant address)
    {
        //for those structure type
        int id = qRegisterMetaType<T>();
        T initialValue; //allocate initial value
        dataMap[address] = QVariant::fromValue(initialValue); //take into map
    }
    int registerBinding(QList<QPair<QVariant,QVariant>> list); //address,value pair

    //!3
    //! Configuration (clusters
    void configureClusters(QList<ModbusClusterConfiguration> configrationList);

    //!4
    //! Tool function
    int queryCluster(QVariant address); //return cluster id

signals:
    void clusterUpdated(int clusterId);
public slots:

    //! Source : cluster
    //! \brief clusterUpdated
    void clusterUpdated();
    //! Source : cluster
    //! \brief requestRaised
    //! \param request
    //!
    void requestRaised(const ModbusSegment* request); //from cluster
protected:

    QMap<int,QVariant> dataMap;

    QList<ModbusCluster*> clusterCollection; //index as cluster id
    quint16 memory[INT16_MAX];

    ModbusSerializedClient* requestGateWay;
};

#endif // CHANNEL_H
