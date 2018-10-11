#include "adsclient.h"
#include <qdebug.h>

AdsClient::AdsClient(AmsAddr address, bool isLocal, long group, long baseOffset, QObject *parent):
    InterfaceClient(parent),
    m_group(group),
    m_amsAddress(address),
    m_baseOffset(baseOffset)
{
    m_client = AdsPortOpen();
    if (isLocal)
        lastResult = AdsGetLocalAddress(&m_amsAddress);
    //!TC3 PLC port = 851
    m_amsAddress.port = 851;
}

AdsClient::~AdsClient()
{
    m_client = AdsPortClose();
}

void AdsClient::onPopRequest()
{
    //InterfaceClient::onPopRequest();
    //!Automatic polling by request/acknowledge
    if(m_queue.isEmpty())
        return;
//    m_isProcessing = true;
//    QtConcurrent::run(this, &AdsClient::executeRequest); //async call
    executeRequest();
}

void AdsClient::executeRequest()
{
    ADDRESS_MODE __address = m_queue.head().Address();
    auto m_register = ADDRESS_REGISTER(__address);
    auto m_size = utilities::sizeOf(m_queue.head().Data());

    switch (m_queue.head().Access())
	{
    case InterfaceRequest::READ:
        lastResult = AdsSyncReadReq(&m_amsAddress,
            m_group,
            m_baseOffset + m_register * 2, //since counting in byte
            m_size,
            m_queue.head().Data().data());
		break;
    case InterfaceRequest::WRITE:
        lastResult = AdsSyncWriteReq(&m_amsAddress,
            m_group,
            m_baseOffset + m_register * 2,
            m_size,
            m_queue.head().Data().data());
		break;
	default:
		break;
	}

    QVariant tempData = m_queue.head().Data();
	//check if result success
	//once done , remove first entity from queue
    switch (lastResult)
	{
	case ADSERR_NOERR:
        //once it is read request , return read value back along with signal (had been processed by ADS API)
        operationDone();
		m_workingTimer->setInterval(0);
		break;
	default:
        qDebug() << QString("ADS Error:%1").arg(lastResult);
		m_workingTimer->setInterval(1000);
		break;
	}
}
