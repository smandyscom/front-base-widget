#ifndef LOADINGHELPER_H
#define LOADINGHELPER_H

#include <QObject>

#include <frontaxisparameter.h>
#include <frontcylinderpanel.h>
#include <frontunitpanel.h>
#include <frontconfigurationtransfer.h>
#include <frontmainpanel.h>

#include <junctionbankdatabase.h>

#include <controllerbase.h>

//!
//! \brief The LoadingHelper class
//! Collection of fronts , build routine loading
class LoadingHelper : public QObject
{
    Q_OBJECT
public:
    explicit LoadingHelper(QObject *parent = nullptr);

    static void LoadFrontAxis(FrontAxisParameter* widget);
    static void LoadFrontCylinder(FrontCylinderPanel* widget);
    static void LoadFrontUnit(FrontUnitPanel* widget);
    static void LoadFrontConfiguration(FrontConfigurationTransfer* widget);
    static void LoadFrontMain(FrontMainPanel* widget);

    static void CombineModelViewV1(FrontAxisParameter* widget1,
                                   FrontCylinderPanel* widget2,
                                   FrontUnitPanel* widget3,
                                   FrontConfigurationTransfer* widget4,
                                   FrontMainPanel* widget5);

    static void CrossLink(ControllerBase* controller,FrontCommon* front);

signals:

public slots:

protected:

    static JunctionBankDatabase* m_instance;
};

#endif // LOADINGHELPER_H
