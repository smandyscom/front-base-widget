#ifndef LOADINGHELPER_H
#define LOADINGHELPER_H

#include <QObject>

#include <frontaxisparameter.h>
#include <frontcylinderpanel.h>
#include <frontunitpanel.h>
#include <frontconfigurationtransfer.h>
#include <frontmainpanel.h>
#include <frontiooverride.h>
#include <frontsafetypanel.h>
#include <frontsignal.h>

#include <junctionbankdatabase.h>

#include <messagehandler.h>
#include <messagereceiver.h>
#include <messagerecorder.h>
//#include <controllerbase.h>

//!
//! \brief The LoadingHelper class
//! Collection of fronts , build routine loading
class LoadingHelper : public QObject
{
    Q_OBJECT
public:
    explicit LoadingHelper(QObject *parent = nullptr);

	static void LoadBaseDatabase();

    static void LoadFrontAxis(FrontAxisParameter* widget);
    static void LoadFrontCylinder(FrontCylinderPanel* widget);
    static void LoadFrontUnit(FrontUnitPanel* widget);
    static void LoadFrontConfiguration(FrontConfigurationTransfer* widget);
    static void LoadFrontMain(FrontMainPanel* widget);
	

    static void LoadFrontIO(FrontIoOverride* widget);
	static void LoadSafeIO(FrontSafetyPanel* widget);

    static void CombineModelViewV1(FrontAxisParameter* widget1,
                                   FrontCylinderPanel* widget2,
                                   FrontUnitPanel* widget3,
                                   FrontConfigurationTransfer* widget4,
                                   FrontMainPanel* widget5,
                                   FrontIoOverride* widget6,
		FrontSafetyPanel* widget7,
		FrontSignal* widget8);


signals:

public slots:

protected:

    static JunctionBankDatabase* m_instance;
};

#endif // LOADINGHELPER_H
