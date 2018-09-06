#ifndef ADSCLIENT_H
#define ADSCLIENT_H

#include <QObject>
//#include <QtConcurrent>
#include <QTimer>
#include <interfaceclient.h>

#define BOOL bool
#include <TcAdsDef.h>
#include <TcAdsAPI.h>
//#pragma comment(lib,"TcAdsDll.lib")


class AdsClient : public InterfaceClient
{
	Q_OBJECT

public:
    AdsClient(AmsAddr address,bool isLocal = false,long group=0x4020,QObject *parent=nullptr);
	~AdsClient();

protected slots:
    void onPopRequest() Q_DECL_OVERRIDE ;
protected:
    AmsAddr __amsAddress;
    long __group;
    long __client;

	void executeRequest();
    long lastResult;

};

#endif // ADSCLIENT_H
