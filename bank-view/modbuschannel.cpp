#include "modbuschannel.h"
#include <utilities.h>
modbusChannel::modbusChannel(QModbusClient *driver, int serverAddress, QObject *parent) :
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

void modbusChannel::beginReadData(QVariant address)
{
    clusterCollection[queryCluster(address)]->beginUpdate();
}

void modbusChannel::writeData(QVariant address, QVariant value)
{
    //fetch the data size
    memcpy(toStartAddress(address),
           value.data(),
           utilities::sizeOf(value));
    //
}

QVariant modbusChannel::readData(QVariant address)
{
    memcpy(dataMap[address].data(),
           toStartAddress(address),
           utilities::sizeOf(dataMap[address]));
    return dataMap[address];
}


void modbusChannel::configureClusters(QList<ModbusClusterConfiguration> configureList)
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

int modbusChannel::queryCluster(QVariant address)
{
    auto registerAddress = address.value<BaseLayer::ModbusDriverAddress>().registerAddress;
    for(int i=0;i<clusterCollection.length();i++){
        registerAddress -= clusterCollection[i]->sizeOf();
        if(registerAddress <=0)
            return i; //on the bound or inside
    }

}

void modbusChannel::requestRaised(const ModbusSegment *request)
{
    requestGateWay->pushRequest(request);
}

quint16* modbusChannel::toStartAddress(QVariant address)
{
    return &memory[address.value<BaseLayer::ModbusDriverAddress>().registerAddress];
}

void modbusChannel::clusterUpdated()
{
    //findout which cluster updated
    auto cluster = qobject_cast<ModbusCluster*>(sender());
    emit clusterUpdated(clusterCollection.indexOf(cluster));
}
