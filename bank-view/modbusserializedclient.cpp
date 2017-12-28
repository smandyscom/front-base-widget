#include "modbusserializedclient.h"

ModbusSerializedClient::ModbusSerializedClient(QModbusClient *driver, int serverAddress, QObject *parent) :
    QObject(parent),
    driver(driver),
    serverAddress(serverAddress)
{
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(popRequest()));
    timer->start();
}


//this function would be poll repeatly
void ModbusSerializedClient::popRequest()
{
    const ModbusSegment* request = requestQueue.head();
    const ModbusSegment::AccessMethod method = request->getMethod();


    if(request == nullptr)
        return; // nothing left in queue

    QModbusReply* reply;

    switch (method) {
    case ModbusSegment::READ:
        reply = driver->sendReadRequest(request->getRequest(),serverAddress);
        break;
    case ModbusSegment::WRITE:
        reply = driver->sendWriteRequest(request->getRequest(),serverAddress);
        break;
    case ModbusSegment::READ_WRITE:
        //reserved
        break;
    default:
        break;
    }

    //for reading , need to connect with request
    if(method==ModbusSegment::READ){
        connect(reply,SIGNAL(finished()),request,SLOT(replyfinished())); //
    }
    //when finished , dequeue
    //otherwise , would remained on queue
    //when use lambda , do not use SIGNAL marco
    connect(reply,&QModbusReply::finished,this,[this,reply](){
        switch (reply->error()) {
        case QModbusDevice::NoError:
            requestQueue.dequeue();
            break;
        default:
            break;
        }
        reply->deleteLater();
    });
}

void ModbusSerializedClient::pushRequest(const ModbusSegment* request)
{
    requestQueue.enqueue(request);
}
