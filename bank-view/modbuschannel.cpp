#include "modbuschannel.h"

modbusChannel::modbusChannel(QObject *parent) : QObject(parent)
{

}

void modbusChannel::beginReadData(QVariant address)
{

}

void modbusChannel::writeData(QVariant address, QVariant value)
{
    quint16* startAddress = &memory[address.value<BaseLayer::ModbusDriverAddress>().registerAddress];

    //
}

QVariant modbusChannel::readData(QVariant address)
{

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
}

int modbusChannel::queryCluster(QVariant address)
{
    auto registerAddress = address.value<BaseLayer::ModbusDriverAddress>().registerAddress;
    for(int i=0;i<clusterCollection.length();i++){
        registerAddress -= clusterCollection[i];
        if(registerAddress <=0)
            return i; //on the bound or inside
    }

}

void modbusChannel::requestRaised(const ModbusSegment *request)
{
    requestGateWay->pushRequest(request);
}
