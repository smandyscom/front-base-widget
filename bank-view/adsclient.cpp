#include "adsclient.h"
#include <qdebug.h>

AdsClient::AdsClient(AmsAddr address, bool isLocal, QObject *parent):
    QObject(parent),
    __address(address)
{
    __client = AdsPortOpen();
    if (isLocal)
        lastResult = AdsGetLocalAddress(&__address);
    //!TC3 PLC port = 851
    __address.port = 851;
    //!
    __timer = new QTimer(this);
    connect(__timer,SIGNAL(timeout()),this,SLOT(onPopRequest()));
    __isProcessing = false;
}

AdsClient::~AdsClient()
{
    __client = AdsPortClose();
}

void AdsClient::onPopRequest()
{
    if (requestQueue.isEmpty() || __isProcessing)
		return;
    __isProcessing = true;
	QtConcurrent::run(this, &AdsClient::executeRequest);
}

void AdsClient::executeRequest()
{
    switch (requestQueue.head().Access())
	{
	case AdsRequest::READ:
        lastResult = AdsSyncReadReq(&__address,
            requestQueue.head().Group(),
			requestQueue.head().Offset(), 
			requestQueue.head().Length(), 
			requestQueue.head().Cache());
		break;
	case AdsRequest::WRITE:
        lastResult = AdsSyncWriteReq(&__address,
            requestQueue.head().Group(),
			requestQueue.head().Offset(),
			requestQueue.head().Length(),
			requestQueue.head().Cache());
		break;
	default:
		break;
	}

	//check if result success
	//once done , remove first entity from queue
    switch (lastResult)
	{
	case ADSERR_NOERR:
        //once it is read request , return read value back along with signal (had been processed by ADS API)
        __aquiredPack = requestQueue.dequeue();
        __isProcessing = false;
        if(__aquiredPack.Access() == AdsRequest::READ)
            emit readRequestDone(__aquiredPack);
		break;
	default:
        qDebug() << "ADS Error:" + lastResult;
		break;
	}
}

void AdsClient::pushRequest(AdsRequest request)
{
    requestQueue.enqueue(request);
}
