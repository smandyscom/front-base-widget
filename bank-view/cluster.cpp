#include "cluster.h"

Cluster::Cluster(quint16* underlyingBaseAddress,
                             size_t size,
                             size_t offset /*base offset in modbus holding register*/,
                             SerializedModbusClient* gateway,
                             QObject *parent) : QObject(parent)
{
    this->baseAddress = underlyingBaseAddress;
    this->size = size;
    this->attribute = attribute;
    this->request=  new SegmentModbus(SegmentModbus::WRITE,
                                      QModbusDataUnit(QModbusDataUnit::HoldingRegisters,offset,size));

}


const quint16* Cluster::read(quint32 offset)
{
    if(attribute.testFlag(READ)){
        request->setMethod(SegmentModbus::READ);
        gateway->pushRequest(request);
        connect(request,SIGNAL(update(QModbusDataUnit)),this,SLOT(dataUpdated(QModbusDataUnit)));
    }

    return &baseAddress[offset];
}

void Cluster::write(quint32 offset, const quint16 *data, size_t sz)
{
    if (!attribute.testFlag(WRITE))
            return ; // no write attribute , cannot perform writing

    memcpy(baseAddress,data,sizeof(quint16)*sz); //store into underlying memory
    for(int i=0;i<sz;i++)
        request->getRequest().setValue(i,baseAddress[i]);

    request->setMethod(SegmentModbus::WRITE);
    gateway->pushRequest(request); //send write request
}

void Cluster::dataUpdated(QModbusDataUnit reply)
{
    //update
    memcpy(baseAddress,reply.values().data(),sizeof(quint16)*sz);
    disconnect(request,SIGNAL(update(QModbusDataUnit)),this,SLOT(dataUpdated(QModbusDataUnit)));
    emit updated();
}
