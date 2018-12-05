#pragma once

#include <QObject>

#include <junctionbankdatabase.h>

#include <interfacechannel.h>
#include <interfaceclient.h>
#include <interfacerequest.h>
#include <adsclient.h>

#include <qsqltablemodel.h>

class LoadingHelperInterface : public QObject
{
	Q_OBJECT

public:
	LoadingHelperInterface(QObject *parent=nullptr);
	~LoadingHelperInterface();

	void LoadingInterfaceV1();
};
