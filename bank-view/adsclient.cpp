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
    //InterfaceClient::onPopRequest();
    //!Automatic polling by request/acknowledge
    if(__queue.isEmpty() || __isProcessing)
        return;
    __isProcessing = true;
//    QtConcurrent::run(this, &AdsClient::executeRequest); //async call
    executeRequest();
}

void AdsClient::executeRequest()
{
    ADDRESS_MODE __address = __queue.head().Address();
    auto __register = ADDRESS_REGISTER(__address);
    auto __size = utilities::sizeOf(__queue.head().Data());

    switch (__queue.head().Access())
	{
    case InterfaceRequest::READ:
        lastResult = AdsSyncReadReq(&__amsAddress,
            __group,
            __register * 2, //since counting in byte
            __size,
            __queue.head().Data().data());
		break;
    case InterfaceRequest::WRITE:
        lastResult = AdsSyncWriteReq(&__amsAddress,
            __group,
            __register * 2,
            __size,
            __queue.head().Data().data());
		break;
	default:
		break;
	}

    QVariant tempData = __queue.head().Data();
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
