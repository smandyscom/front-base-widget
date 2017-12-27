#include "serializedmodbusclient.h"

SerializedModbusClient::SerializedModbusClient(QObject *parent) : QObject(parent)
{
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(popRequest()));
    timer->start();
}


//this function would be poll repeatly
void SerializedModbusClient::popRequest()
{
    SegmentModbus* request = queue.head();
    if(request == nullptr)
        return; // nothing left in queue

    QModbusReply* reply;

    switch (request->getMethod()) {
    case SegmentModbus::READ:
        reply = gateway->sendReadRequest(request->getRequest(),serverAddress);
        break;
    case SegmentModbus::WRITE:
        break;
    case SegmentModbus::READ_WRITE:
        //reserved
        break;
    default:
        break;
    }

    //for reading , need to connect with request
    if(request->getMethod()==SegmentModbus::READ){
        connect(reply,SIGNAL(finished()),request,SLOT(replyfinished())); //
    }
    //when finished , dequeue
    //otherwise , would remained on queue
    //when use lambda , do not use SIGNAL marco
    connect(reply,&QModbusReply::finished,this,[this,reply](){
        switch (reply->error()) {
        case QModbusDevice::NoError:
            queue.dequeue()->deleteLater(); //life ends as long as operation done
            break;
        default:
            break;
        }
        reply->deleteLater();
    });
}

void SerializedModbusClient::pushRequest(SegmentModbus* request)
{
    queue.enqueue(request);
}
