#include "modbusserializedclient.h"
#include <QDebug>
ModbusSerializedClient::ModbusSerializedClient(int serverAddress, QObject *parent) :
    QObject(parent),
    __serverAddress(serverAddress)
{
    __driver = nullptr;
    //!
    __timer = new QTimer(this);
    connect(__timer,SIGNAL(timeout()),this,SLOT(onPopRequest()));
    __isProcessing = false;
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


//    qDebug() << "destructor";
//    //! release all rest requests
//    while (requestQueue.count() > 0 &&
//           __driver->state()==QModbusDevice::ConnectedState) {
//        __isProcessing = false;//forced clear out
//        onPopRequest();
//        requestQueue.dequeue();
//    }
//    qDebug() << "destructor";
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
            if(requestQueue.head()->second==READ)
                emit readRequestDone(reply->result());
            //destroy
            requestQueue.dequeue();
            break;
        default:

            qDebug() << reply->errorString();
            break;
        }//switch

        reply->deleteLater();
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
        qDebug() << __driver->connectionParameter(QModbusDevice::NetworkAddressParameter)
                 << ":"
                 << __driver->connectionParameter(QModbusDevice::NetworkPortParameter)
                 << "Connected" ;
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
