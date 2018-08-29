#ifndef ADSCLIENT_H
#define ADSCLIENT_H

#include <QObject>
#include <QtConcurrent>
#include <QTimer>
#include <interfaceclient.h>

#define BOOL bool
#include <TcAdsDef.h>
#include <TcAdsAPI.h>
//#pragma comment(lib,"TcAdsDll.lib")

#include <adsRequest.h>

class AdsClient : public InterfaceClient
{
	Q_OBJECT

public:
    AdsClient(AmsAddr address,bool isLocal = false,QObject *parent=nullptr);
	~AdsClient();

    void pushRequest(AdsRequest request);


signals:
    void readRequestDone(AdsRequest pack);
private:
	//mind how much size can be aquired at once in ADS protocal

protected slots:
	void onPopRequest();
	//void onDriverStateChanged(QModbusDevice::State state);
	//void onDriverErrorOccured(QModbusDevice::Error error);
protected:
	QQueue<AdsRequest> requestQueue; //TODO Ads request
	QTimer* __timer; //driving consumer
	AmsAddr __address;
    long __client;

    bool __isProcessing;
	void executeRequest();
    long lastResult;

    AdsRequest __aquiredPack;
};

#endif // ADSCLIENT_H
