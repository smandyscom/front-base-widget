#include "adsclient.h"

AdsClient::AdsClient(QObject *parent)
	: QObject(parent)
{
}

AdsClient::~AdsClient()
{

}

void AdsClient::onPopRequest()
{
	long result = 0;
	int value;
	result = AdsSyncReadReq(&__address, 0x4020, 0x0, 0x4, static_cast<void*>(&value));

}

