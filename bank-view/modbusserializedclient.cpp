#include "modbusserializedclient.h"
#include <QDebug>
ModbusSerializedClient::ModbusSerializedClient(int serverAddress, QObject *parent) :
    QObject(parent),
    __serverAddress(serverAddress),
    __isDestroyed(false)
{
    __driver = nullptr;
    //!
    __timer = new QTimer(this);
    connect(__timer,SIGNAL(timeout()),this,SLOT(onPopRequest()));
    __isProcessing = false;

    connect(this,&ModbusSerializedClient::destroyed,[=]{
       //qDebug() << "";
       if(__driver != nullptr)
       {
           disconnect(__driver,&QModbusClient::stateChanged,this,&ModbusSerializedClient::onDriverStateChanged);
           disconnect(__driver,&QModbusClient::errorOccurred,this,&ModbusSerializedClient::onDriverErrorOccured);
           //disconnect(reply,&QModbusReply::finished,this,&ModbusSerializedClient::onReplyFinished);
           //disconnect(__driver,SIGNAL(stateChanged(QModbusDevice::State)),this,SLOT(onDriverStateChanged(QModbusDevice::State)));
           //disconnect(__driver,SIGNAL(errorOccurred(QModbusDevice::Error)),this,SLOT(onDriverErrorOccured(QModbusDevice::Error)));
           __isDestroyed = true;
       }
    });
}

void ModbusSerializedClient::Driver(QModbusClient* value)
{
    if(__driver != nullptr)
        return;
    __driver = value;

    connect(__driver,SIGNAL(stateChanged(QModbusDevice::State)),this,SLOT(onDriverStateChanged(QModbusDevice::State)));
    connect(__driver,SIGNAL(errorOccurred(QModbusDevice::Error)),this,SLOT(onDriverErrorOccured(QModbusDevice::Error)));
}

ModbusSerializedClient::~ModbusSerializedClient()
{
    //! Message loop stuck here , so that requests cannot be comsumed
    qDebug()<<"";
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
    reply = nullptr;
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
    connect(reply,&QModbusReply::finished,this,&ModbusSerializedClient::onReplyFinished);
}

void ModbusSerializedClient::pushRequest(ModbusRequest *request)
{
    requestQueue.enqueue(request);
}

void ModbusSerializedClient::onDriverStateChanged(QModbusDevice::State state)
{
    switch (state) {
    case QModbusDevice::ConnectedState:
        qDebug() << __driver->connectionParameter(QModbusDevice::NetworkAddressParameter)
                 << ":"
                 << __driver->connectionParameter(QModbusDevice::NetworkPortParameter)
                 << "Connected , with interval:"
                 << __timer->interval();
        __timer->start();
        break;
    case QModbusDevice::UnconnectedState:
        //! Reconnect
        QTimer::singleShot(1000,[=](){
            __driver->connectDevice();
        });
    default:
        break;
    }
}

void ModbusSerializedClient::onDriverErrorOccured(QModbusDevice::Error error)
{
    switch (error) {
    case QModbusDevice::ConnectionError:
    case QModbusDevice::ReplyAbortedError:
        __timer->stop();
        break;
        //! Drop invalid request when occured
    default:
        break;
    }
}

void ModbusSerializedClient::onReplyFinished()
{
    if(__isDestroyed)
        return; //would be called by another thread

    switch (reply->error()) {
    case QModbusDevice::NoError:
    {
        if(requestQueue.head()->second==READ)
            emit readRequestDone(reply->result());
        //destroy
        ModbusRequest* __request = requestQueue.dequeue();
        delete __request;
        break;
    }
    default:

        qDebug() << reply->errorString();
        break;
    }//switch

    reply->deleteLater();
    __isProcessing = false;
}
