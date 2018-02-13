#include "modbuscluster.h"

ModbusCluster::ModbusCluster(quint16* innerBaseAddress,
                             size_t size,
                             size_t modBusStartAddress /*base offset in modbus holding register*/,
                             Attributes attribute,
                             QObject *parent) :
    QObject(parent),
    innerBaseAddress(innerBaseAddress),
    size(size),
    attribute(attribute)
{
    this->request=  new ModbusSegment(ModbusSegment::WRITE,
                                      QModbusDataUnit(QModbusDataUnit::HoldingRegisters,
                                                      modBusStartAddress,
                                                      size));

}


void ModbusCluster::beginUpdateCluster()
{
    //once has READ attribute
    if(!attribute.testFlag(READ))
        return;

    request->setMethod(ModbusSegment::READ);
    connect(request,&ModbusSegment::update,this,&ModbusCluster::onModbusReplyReceived);
    emit sendModbusRequest(request);
}

void ModbusCluster::commitCluster()
{
    if (!attribute.testFlag(WRITE))
            return ; // no write attribute , cannot perform writing

    for(int i=0;i<size;i++)
        request->requestUnit.setValue(i,innerBaseAddress[i]);

    request->setMethod(ModbusSegment::WRITE);
    emit sendModbusRequest(request);//send write request
}

void ModbusCluster::onModbusReplyReceived(QModbusDataUnit reply)
{
    //update
    memcpy(innerBaseAddress,reply.values().data(),sizeof(quint16)*size);
    disconnect(request,&ModbusSegment::update,this,&ModbusCluster::onModbusReplyReceived);
    emit clusterUpdated();
}
