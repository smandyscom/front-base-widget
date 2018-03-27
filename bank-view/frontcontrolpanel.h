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
private:
    Ui::frontControlPanel *ui;

    ControllerMainPanel* __controller;
    QTimer* __timer;
};

#endif // FRONTCONTROLPANEL_H
