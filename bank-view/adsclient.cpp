#include "adsclient.h"
#include <qdebug.h>

AdsClient::AdsClient(AmsAddr address, bool isLocal, long group, QObject *parent):
    InterfaceClient(parent),
    __group(group),
    __amsAddress(address)
{
    __client = AdsPortOpen();
    if (isLocal)
        lastResult = AdsGetLocalAddress(&__amsAddress);
    //!TC3 PLC port = 851
    __amsAddress.port = 851;
}

AdsClient::~AdsClient()
{
    __client = AdsPortClose();
}

void AdsClient::onPopRequest()
{
    InterfaceClient::onPopRequest();
    QtConcurrent::run(this, &AdsClient::executeRequest); //async call
}

void AdsClient::executeRequest()
{
    ADDRESS_MODE __address = __queue.head().Address();
    auto temp = ADDRESS_REGISTER(__address);
    QVariant tempData = __queue.head().Data();
    switch (__queue.head().Access())
	{
    case InterfaceRequest::READ:
        lastResult = AdsSyncReadReq(&__amsAddress,
            __group,
            temp,
            utilities::sizeOf(__queue.head().Data()),
            __queue.head().Data().data());
		break;
    case InterfaceRequest::WRITE:
        lastResult = AdsSyncWriteReq(&__amsAddress,
            __group,
            ADDRESS_REGISTER(__address),
            utilities::sizeOf(__queue.head().Data()),
            __queue.head().Data().data());
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
        operationDone();
		break;
	default:
        qDebug() << "ADS Error:" + lastResult;
		break;
	}
}
