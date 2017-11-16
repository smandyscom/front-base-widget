#ifndef MODBUSSETUPWIDGET_H
#define MODBUSSETUPWIDGET_H

#include <QWidget>
#include <QModbusDevice>
#include <QStandardItemModel>
#include <bank-view_global.h>

namespace Ui {
class ModbusSetupWidget;
}

class BANKVIEWSHARED_EXPORT ModbusSetupWidget : public QWidget
{
    Q_OBJECT

public:
    enum test{
      A
    };
    Q_ENUM(test)

    explicit ModbusSetupWidget(QWidget *parent = 0);
    ~ModbusSetupWidget();

private:
    Ui::ModbusSetupWidget *ui;

    //abstrct device , instance would decided by current setting
    QModbusDevice* __device;

    QStandardItemModel* settingModel;
};

#endif // MODBUSSETUPWIDGET_H
