#ifndef LOADINGHELPERCONTROLLERS_H
#define LOADINGHELPERCONTROLLERS_H

#include <QObject>

#include <frontcommon.h>

#include <definitionaxisblocks.h>
#include <definitioncommandblock.h>
#include <definitioncylinderblock.h>
#include <definitionsignalblock.h>
#include <definitionunitblocks.h>
#include <definitionunitconfigblock.h>
#include <definitionioattributes.h>

#include <controllerbanktransfer.h>
#include <controllermainpanel.h>
#include <controlleriomonitor.h>
#include <controllermaterialtransfer.h>

#include <junctionbankdatabase.h>
#include <junctionmaterialdatabase.h>

#include <propertyportcommon.h>

class LoadingHelperControllers : public QObject
{
    Q_OBJECT
public:
    explicit LoadingHelperControllers(QObject *parent = nullptr);

    void LoadTransfer(quint8 clientId, quint16 baseOffset, int interval);
    void LoadInputsMonitor();
    void LoadOutputsMonitor();
    void LoadCylinderMonitor();
	void LoadMaterialTransfer();

	void LoadAdaptors();

    void ControllersLoadingRoutineV1();

    JunctionBankDatabase* m_database;
    ControllerBankTransfer* m_controllerTransfer;
    ControllerMainPanel* m_controllerMain;
	QList<ControllerMaterialTransfer*> m_controllersMaterial;

    ControllerIOMonitor* m_controllerInputMonitor;
    ControllerIOMonitor* m_controllerOutputMonitor;
    ControllerIOMonitor* m_controllerCylinderMonitor;
	
	QList<AbstractSqlTableAdpater*> m_adaptorList;

    //void CrossLink(ControllerBase* controller,FrontCommon* front);
	void CrossLink(QObject* port1, QObject* port2);
	
signals:
	void controllerLoaded();
public slots:

protected:

};

#endif // LOADINGHELPERCONTROLLERS_H
