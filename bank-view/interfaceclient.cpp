#include "interfaceclient.h"

InterfaceClient::InterfaceClient(QObject *parent) :
    QObject(parent),
    __isProcessing(false)
{
    //! Initialize internal cache
    __cache = new quint16[USHRT_MAX];
    memset(__cache,0,USHRT_MAX);
    //! create timer
    __workingTimer = new QTimer(this);
    connect(__workingTimer,SIGNAL(timeout()),this,SLOT(onPopRequest()));
}

void InterfaceClient::pushRequest(InterfaceRequest request)
{
    __queue.enqueue(request);
}

void InterfaceClient::operationDone()
{
    //! dequeue from head
    InterfaceRequest ack = __queue.dequeue(); //when fetched data , put into head
    __isProcessing = false;
    //! write into cache
    ADDRESS_MODE __address = ack.Address();
    memcpy(&__cache[ADDRESS_REGISTER(__address)],ack.Data().data(),utilities::sizeOf(ack.Data()));
    emit requestAcknowledged(ack);
}

void InterfaceClient::onPopRequest()
{
    if(__queue.isEmpty() || __isProcessing)
        return;
    __isProcessing = true;
}
