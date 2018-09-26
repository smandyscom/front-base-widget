#ifndef FRONTSAFETYPANEL_H
#define FRONTSAFETYPANEL_H

#include <QWidget>
#include <QTimer>

#include <junctionbankdatabase.h>
#include <tablemodeliooverride.h>

#include <definitionioattributes.h>.h>
#include <definitionauxiliarykeys.h>

#include <controllersafety.h>

#include <utilities.h>

#include <abstractauthreceiver.h>

namespace Ui {
class FrontSafetyPanel;
}

class FrontSafetyPanel : public QWidget , public AbstractAuthReceiver
{
    Q_OBJECT

public:
    explicit FrontSafetyPanel(QWidget *parent = 0);
    ~FrontSafetyPanel();
protected slots:
   void onInitialized();
private:
    Ui::FrontSafetyPanel *ui;

    TableModelIOOverride* __monitorTable;

    ControllerSafety* __controller;
};

#endif // FRONTSAFETYPANEL_H
