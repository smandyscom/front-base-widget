#ifndef LOADINGHELPERCONTROLLERS_H
#define LOADINGHELPERCONTROLLERS_H

#include <QObject>

#include <frontcommon.h>

#include <definitionaxisblocks.h>
#include <definitioncommandblock.h>
#include <definitioncylinderblock.h>
#include <definitionsignalblock.h>
#include <definitionunitblocks.h>
#include <definitionioattributes.h>

#include <controllerbanktransfer.h>
#include <controllermainpanel.h>
#include <controlleriomonitor.h>

#include <junctionbankdatabase.h>

class LoadingHelperControllers : public QObject
{
    Q_OBJECT
public:
    explicit LoadingHelperControllers(QObject *parent = nullptr);

    static void LoadTransfer(quint8 clientId, quint16 baseOffset, int interval);
    static void LoadInputsMonitor();
    static void LoadOutputsMonitor();
    static void LoadCylinderMonitor();

    static void ControllersLoadingRoutineV1();

    static JunctionBankDatabase* m_database;
    static ControllerBankTransfer* m_controllerTransfer;
    static ControllerMainPanel* m_controllerMain;

    static ControllerIOMonitor* m_controllerInputMonitor;
    static ControllerIOMonitor* m_controllerOutputMonitor;
    static ControllerIOMonitor* m_controllerCylinderMonitor;


    static void CrossLink(ControllerBase* controller,FrontCommon* front);

signals:

public slots:

protected:

};

#endif // LOADINGHELPERCONTROLLERS_H
