#include "modbuscluster.h"

ModbusCluster::ModbusCluster(quint16* baseAddress,
                 size_t size,
                 size_t registerAddress /*base offset in modbus holding register*/,
                 QObject *parent) : QObject(parent)
{
    this->baseAddress = baseAddress;
    this->size = size;
    this->attribute = attribute;
    this->request=  new ModbusSegment(ModbusSegment::WRITE,
                                      QModbusDataUnit(QModbusDataUnit::HoldingRegisters,offset,size));

}


const void ModbusCluster::beginUpdate()
{
    //once has READ attribute
    if(attribute.testFlag(READ)){
        request->setMethod(ModbusSegment::READ);
        requestGateway->pushRequest(request);
        connect(request,SIGNAL(beginUpdate(QModbusDataUnit)),this,SLOT(dataUpdated(QModbusDataUnit)));
    }

    emit sendRequest(request);
}

void ModbusCluster::commit()
{
    if (!attribute.testFlag(WRITE))
            return ; // no write attribute , cannot perform writing

    for(int i=0;i<size;i++)
        request->getRequest().setValue(i,baseAddress[i]);

    request->setMethod(ModbusSegment::WRITE);
    requestGateway->pushRequest(request); //send write request

    emit sendRequest(request);
}

void ModbusCluster::dataUpdated(QModbusDataUnit reply)
{
    //update
    memcpy(baseAddress,reply.values().data(),sizeof(quint16)*size);
    disconnect(request,SIGNAL(beginUpdate(QModbusDataUnit)),this,SLOT(dataUpdated(QModbusDataUnit)));
    emit updated();
}
