#include "interfaceclient.h"

InterfaceClient::InterfaceClient(QObject *parent) :
    QObject(parent),
    m_isProcessing(false)
{
    //! Initialize internal cache
    m_cache = new quint16[USHRT_MAX];
    memset(m_cache,0,USHRT_MAX);
    //! create timer
    m_workingTimer = new QTimer(this);
    connect(m_workingTimer,SIGNAL(timeout()),this,SLOT(onPopRequest()));
    m_workingTimer->start();
}

void InterfaceClient::pushRequest(InterfaceRequest request)
{
    m_queue.enqueue(request);
}

void InterfaceClient::operationDone()
{
    //! dequeue from head
    InterfaceRequest ack = m_queue.dequeue(); //when fetched data , put into head
    m_isProcessing = false;
    //! write into cache
    ADDRESS_MODE __address = ack.Address();
    memcpy(&m_cache[ADDRESS_REGISTER(__address)],ack.Data().data(),utilities::sizeOf(ack.Data()));

    emit requestAcknowledged(ack);
}

void InterfaceClient::onPopRequest()
{
    //!Automatic polling by request/acknowledge
    if(m_queue.isEmpty() || m_isProcessing)
        return;
    m_isProcessing = true;
//    onPopRequest();
}

bool InterfaceClient::IsConnected() const
{
	return false;
}