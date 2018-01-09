#include "modbuschannel.h"
#include <utilities.h>
#include <QCoreApplication>

ModbusChannel::ModbusChannel(QModbusClient *driver, int serverAddress, QObject *parent) :
    QObject(parent)
{
    if(driver != nullptr){
        requestGateWay = new ModbusSerializedClient(driver,
                                                    serverAddress,
                                                    this);
    }
    else{
        requestGateWay = nullptr; //no gateway, simulation only
    }
}

void ModbusChannel::beginReadData(ModbusDriverAddress address)
{
    clusterCollection[queryCluster(address)]->beginUpdate();
}

void ModbusChannel::writeData(ModbusDriverAddress address, QVariant value)
{
    //fetch the data size
    memcpy(toStartAddress(address),
           value.data(),
           utilities::sizeOf(value));
    //
}

QVariant ModbusChannel::readData(ModbusDriverAddress address)
{
    memcpy(dataMap[address].data(),
           toStartAddress(address),
           utilities::sizeOf(dataMap[address]));
    return dataMap[address];
}


void ModbusChannel::configureClusters(QList<ModbusClusterConfiguration> configureList)
{
    int startAddress = 0;
    for(int i=0;i<configureList.length();i++){
        clusterCollection.append(new ModbusCluster(&memory[startAddress],
                                                   configureList[i].second,
                                                   startAddress,
                                                   configureList[i].first,
                                                   this));
        startAddress+=configureList[i].second; //accumulate
    }//for

    //bridge them
    foreach (ModbusCluster* cluster, clusterCollection) {
        if(requestGateWay != nullptr){
            connect(cluster,
                    SIGNAL(sendRequest(const ModbusSegment*)),
                    this,
                    SLOT(requestRaised(const ModbusSegment*)));
        }//gateway existed
        connect(cluster,
                SIGNAL(updated()),
                this,
                SLOT(clusterUpdated()));
    }
}

int ModbusChannel::queryCluster(ModbusDriverAddress address)
{
    auto registerAddress = address.value<BaseLayer::ModbusDriverAddress>().registerAddress;
    for(int i=0;i<clusterCollection.length();i++){
        registerAddress -= clusterCollection[i]->sizeOf();
        if(registerAddress <=0)
            return i; //on the bound or inside
    }

}

void ModbusChannel::requestRaised(const ModbusSegment *request)
{
    requestGateWay->pushRequest(request);
}

quint16* ModbusChannel::toStartAddress(ModbusDriverAddress address)
{
    return &memory[address.registerAddress];
}

//!
//! \brief ModbusChannel::clusterUpdated
//! Dispatch events
void ModbusChannel::clusterUpdated()
{
    //findout which cluster updated
    auto cluster = qobject_cast<ModbusCluster*>(sender());
    emit clusterUpdated(clusterCollection.indexOf(cluster));

    int clusterId = clusterCollection.indexOf(cluster);

    foreach (QVariant &var, dataMap.keys()) {
        if (isInCluster(var,clusterId)){
            emit updated(var,readData((var)));
        }
    }

}
