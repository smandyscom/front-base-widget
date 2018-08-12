#ifndef ADSCLIENT_H
#define ADSCLIENT_H

#include <QObject>
#include <QtConcurrent>
#include <QTimer>

#define BOOL bool
#include <TcAdsDef.h>
#include <TcAdsAPI.h>
//#pragma comment(lib,"TcAdsDll.lib")

class AdsClient : public QObject
{
	Q_OBJECT

public:
	AdsClient(QObject *parent);
	~AdsClient();

private:
	//mind how much size can be aquired at once in ADS protocal
	//
protected slots:
	void onPopRequest();
	//void onDriverStateChanged(QModbusDevice::State state);
	//void onDriverErrorOccured(QModbusDevice::Error error);
protected:
	QQueue<void*> requestQueue; //TODO Ads request
	QTimer* __timer; //driving consumer
	AmsAddr __address;
};

#endif // ADSCLIENT_H
