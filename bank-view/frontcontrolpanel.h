#ifndef FRONTCONTROLPANEL_H
#define FRONTCONTROLPANEL_H

#include <QWidget>

#include <controllermainpanel.h>
#include <QTimer>
#include <utilities.h>

namespace Ui {
class frontControlPanel;
}

class frontControlPanel : public QWidget
{
    Q_OBJECT

public:
    explicit frontControlPanel(QWidget *parent = 0);
    ~frontControlPanel();

protected slots:
    void onTimerTimeout();
    void onCheckedChanged();
    void onStateChanged(ControllerMainPanel::MainStates currentState);
private:
    Ui::frontControlPanel *ui;

    ControllerMainPanel* __controller;
    QTimer* __timer;

    QList<QWidget*> __manualInterlock;
    QList<QWidget*> __autoInterlock;
};

#endif // FRONTCONTROLPANEL_H
