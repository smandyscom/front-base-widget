#include "adsclient.h"
#include <qdebug.h>
AdsClient::AdsClient(QObject *parent)
	: QObject(parent)
{
}

AdsClient::~AdsClient()
{

}

void AdsClient::onPopRequest()
{
	if (requestQueue.isEmpty() || _isProcessing)
		return;
	_isProcessing = true;
	QtConcurrent::run(this, &AdsClient::executeRequest);
}

void AdsClient::executeRequest()
{
	long result = 0;
	switch (requestQueue.head().Access)
	{
	case AdsRequest::READ:
		result = AdsSyncReadReq(&__address, 
			requestQueue.head().Group, 
			requestQueue.head().Offset(), 
			requestQueue.head().Length(), 
			requestQueue.head().Cache());
		break;
	case AdsRequest::WRITE:
		result = AdsSyncWriteReq(&__address,
			requestQueue.head().Group,
			requestQueue.head().Offset(),
			requestQueue.head().Length(),
			requestQueue.head().Cache());
		break;
	default:
		break;
	}

	//check if result success
	//once done , remove first entity from queue
	switch (result)
	{
	case ADSERR_NOERR:
		//once it is read request , return read value back along with signal (had been processed by ADS API)
		requestQueue.dequeue();
		_isProcessing = false;
		break;
	default:
		qDebug() << "ADS Error:" + result;
		break;
	}
}