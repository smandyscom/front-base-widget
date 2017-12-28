#include "modbusserializedclient.h"

ModbusSerializedClient::ModbusSerializedClient(QModbusClient *driver, QObject *parent) : QObject(parent)
{
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(popRequest()));
    timer->start();

    this->driver = driver;
}


//this function would be poll repeatly
void ModbusSerializedClient::popRequest()
{
    ModbusSegment* request = requestQueue.head();
    if(request == nullptr)
        return; // nothing left in queue

    QModbusReply* reply;

    switch (request->getMethod()) {
    case ModbusSegment::READ:
        reply = driver->sendReadRequest(request->getRequest(),serverAddress);
        break;
    case ModbusSegment::WRITE:
        break;
    case ModbusSegment::READ_WRITE:
        //reserved
        break;
    default:
        break;
    }

    //for reading , need to connect with request
    if(request->getMethod()==ModbusSegment::READ){
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

void ModbusSerializedClient::pushRequest(ModbusSegment* request)
{
    requestQueue.enqueue(request);
}
