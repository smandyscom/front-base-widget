#include "modbusserializedclient.h"

ModbusSerializedClient::ModbusSerializedClient(QModbusClient *driver, int serverAddress, QObject *parent) :
    QObject(parent),
    driverReference(driver),
    serverAddress(serverAddress)
{
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(popRequest()));
    timer->start();
}

//!
//! \brief ModbusSerializedClient::popRequest
//! this function would be poll repeatly
void ModbusSerializedClient::popRequest()
{
    if(requestQueue.isEmpty())
        return;

    const ModbusSegment* request = requestQueue.head();
    const ModbusSegment::AccessMethod method = request->getMethod();

    if(request == nullptr)
        return; // nothing left in queue

    QModbusReply* reply = nullptr;

    switch (method) {
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

    //! pause timer
    timer->stop();

    //! Connect with request
    connect(reply,SIGNAL(finished()),request,SLOT(replyfinished())); //

    //!
    //! when finished , dequeue
    //! otherwise , would remained on queue
    //! when use lambda , do not use SIGNAL marco
    connect(reply,&QModbusReply::finished,this,[this,reply](){
        switch (reply->error()) {
        case QModbusDevice::NoError:
            requestQueue.dequeue();
            break;
        default:
            break;
        }
        reply->deleteLater();
        timer->start();//restart
    });
}

void ModbusSerializedClient::pushRequest(const ModbusSegment* request)
{
    requestQueue.enqueue(request);
}
