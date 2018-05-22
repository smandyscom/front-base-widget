#ifndef FRONTCONTROLPANEL_H
#define FRONTCONTROLPANEL_H

#include <QWidget>

#include <controllermainpanel.h>
#include <QTimer>
#include <utilities.h>

#include <frontsafetypanel.h>

#include <abstractauthreceiver.h>

namespace Ui {
class frontControlPanel;
}

class FrontControlPanel : public QWidget , public AbstractAuthReceiver
{
    Q_OBJECT

public:
    explicit FrontControlPanel(QWidget *parent = 0);
    ~FrontControlPanel();

protected slots:
    void onTimerTimeout();
    void onCheckedChanged(bool toggled);
    void onStateChanged(ControllerMainPanel::MainStates currentState);
    void onErrorChanged(bool currentError);

private:
    Ui::frontControlPanel *ui;

    ControllerMainPanel* __controller;
    QTimer* __timer;

    QList<QWidget*> __manualInterlock;
    QList<QWidget*> __autoInterlock;
};

#endif // FRONTCONTROLPANEL_H
