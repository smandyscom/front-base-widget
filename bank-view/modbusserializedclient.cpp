#include "modbusserializedclient.h"

ModbusSerializedClient::ModbusSerializedClient(QModbusClient *driver, int serverAddress, QObject *parent) :
    QObject(parent),
    driverReference(driver),
    serverAddress(serverAddress)
{
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(popRequest()));
    timer->start();
    isProcessing = false;
}

//!
//! \brief ModbusSerializedClient::popRequest
//! this function would be poll repeatly
void ModbusSerializedClient::popRequest()
{
    if(requestQueue.isEmpty() || isProcessing)
        return;

    //!
    //! Lock-up processor
    //!
    isProcessing = true;

    request = const_cast<ModbusSegment*>(requestQueue.head());

    QModbusReply* reply = nullptr;
    switch (request->getMethod()) {
    case ModbusSegment::READ:
        reply = driverReference->sendReadRequest(request->requestUnit,serverAddress);
        break;
    case ModbusSegment::WRITE:
        reply = driverReference->sendWriteRequest(request->requestUnit,serverAddress);
        break;
    case ModbusSegment::READ_WRITE:
        //reserved
        break;
    default:
        break;
    }
    //!
    //! when finished , dequeue
    //! otherwise , would remained on queue
    //! when use lambda , do not use SIGNAL marco
    connect(reply,&QModbusReply::finished,this,[this,reply](){
        switch (reply->error()) {
        case QModbusDevice::NoError:
            requestQueue.dequeue();
            emit request->update(reply->result());
            reply->deleteLater();
            break;
        default:
            break;
        }  
        isProcessing = false;
    });
}

void ModbusSerializedClient::pushRequest(const ModbusSegment* request)
{
    requestQueue.enqueue(request);
}
