#include "modbusserializedclient.h"
#include <QDebug>
ModbusSerializedClient::ModbusSerializedClient(QModbusClient *driver, int serverAddress, QObject *parent) :
    QObject(parent),
    __driver(driver),
    __serverAddress(serverAddress)
{
    __timer = new QTimer(this);
    connect(__timer,SIGNAL(timeout()),this,SLOT(onPopRequest()));
    connect(__driver,SIGNAL(stateChanged(QModbusDevice::State)),this,SLOT(onDriverStateChanged(QModbusDevice::State)));
    __isProcessing = false;
}

//!
//! \brief ModbusSerializedClient::popRequest
//! this function would be poll repeatly
void ModbusSerializedClient::onPopRequest()
{
    if(requestQueue.isEmpty() || __isProcessing)
        return;

    //!
    //! Lock-up processor
    //!
    __isProcessing = true;

    //request = const_cast<ModbusRequest*>(requestQueue.head());

    QModbusReply* reply = nullptr;
    switch (requestQueue.head()->second) {
    case READ:
        reply = __driver->sendReadRequest(requestQueue.head()->first,__serverAddress);
        break;
    case WRITE:
        reply = __driver->sendWriteRequest(requestQueue.head()->first,__serverAddress);
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

        __isProcessing = false;
    });
}

void ModbusSerializedClient::pushRequest(const ModbusRequest *request)
{
    requestQueue.enqueue(request);
}

void ModbusSerializedClient::onDriverStateChanged(QModbusDevice::State state)
{
    switch (state) {
    case QModbusDevice::ConnectedState:
        __timer->start();
        break;
    case QModbusDevice::ConnectionError:
    case QModbusDevice::ConnectingState:
        __timer->stop();
        break;
    default:
        break;
    }
}
