#include "modbusserializedclient.h"
#include <QDebug>
ModbusSerializedClient::ModbusSerializedClient(QModbusClient *driver, int serverAddress, QObject *parent) :
    QObject(parent),
    driverReference(driver),
    serverAddress(serverAddress)
{
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(onPopRequest()));
    timer->start();
    isProcessing = false;
}

//!
//! \brief ModbusSerializedClient::popRequest
//! this function would be poll repeatly
void ModbusSerializedClient::onPopRequest()
{
    if(requestQueue.isEmpty() || isProcessing)
        return;

    //!
    //! Lock-up processor
    //!
    isProcessing = true;

    //request = const_cast<ModbusRequest*>(requestQueue.head());

    QModbusReply* reply = nullptr;
    switch (requestQueue.head()->second) {
    case READ:
        reply = driverReference->sendReadRequest(requestQueue.head()->first,serverAddress);
        break;
    case WRITE:
        reply = driverReference->sendWriteRequest(requestQueue.head()->first,serverAddress);
        break;
    case READ_WRITE:
        //reserved
        break;
    default:
        break;
    }

    if(reply==nullptr)
        return;

    //!
    //! when finished , dequeue
    //! otherwise , would remained on queue
    //! when use lambda , do not use SIGNAL marco
    connect(reply,&QModbusReply::finished,this,[this,reply](){
        switch (reply->error()) {
        case QModbusDevice::NoError:
            emit requestDone(reply->result());
            //destroy
            requestQueue.dequeue();
            reply->deleteLater();
            break;
        default:
            qDebug() << reply->errorString();
            break;
        }//switch

        isProcessing = false;
    });
}

void ModbusSerializedClient::pushRequest(const ModbusRequest *request)
{
    requestQueue.enqueue(request);
}
