#ifndef MODBUSSETUPWIDGET_H
#define MODBUSSETUPWIDGET_H

#include <QWidget>
#include <QModbusDevice>
#include <QStandardItemModel>
#include <enumcomboboxgeneric.h>

namespace Ui {
class ModbusSetupWidget;
}

class  ModbusSetupWidget : public QWidget
{
    Q_OBJECT

public:
    enum DeviceKinds{
      MASTER_CLIENT_RTUSERIAL,
      MASTER_CLIENT_TCP,
      SLAVE_SERVER_RTUSERIAL,
      SLAVE_SERVER_TCP
    };
    Q_ENUM(DeviceKinds)

    explicit ModbusSetupWidget(QWidget *parent = 0);
    ~ModbusSetupWidget();

    QModbusDevice* Device() const {return __device;}

private slots:
    void onStateChanged(QModbusDevice::State state);
    void onErrorOccured(QModbusDevice::Error error);
    void onSelectionChanged();

    void deviceDisconnect();
    void deviceConnect();
private:
    Ui::ModbusSetupWidget *ui; 

    QModbusDevice* __device;//abstrct device , instance would decided by current setting
    QStandardItemModel* settingModel;



    EnumComboBoxGenericTemplate<DeviceKinds>* deviceSelection;
    QMap<DeviceKinds,QModbusDevice*> deviceTorrent; //used to store diffrent kind of modbus device listed in DeviceKinds
};

#endif // MODBUSSETUPWIDGET_H
