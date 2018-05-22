#ifndef FRONTIOOVERRIDE_H
#define FRONTIOOVERRIDE_H

#include <QWidget>
#include <QSqlRelationalTableModel>
#include <tablemodeliooverride.h>
#include <modbuschannel.h>

#include <frontsinglefilter.h>
#include <junctionbankdatabase.h>

#include <utilities.h>

#include <definitionsignalblock.h>
#include <controllermanualmode.h>

#include <abstractauthreceiver.h>

namespace Ui {
class FrontIoOverride;
}



class FrontIoOverride : public QWidget ,
        public AbstractAuthReceiver
{
    Q_OBJECT

public:
    explicit FrontIoOverride(QWidget *parent = 0);
    ~FrontIoOverride();
protected slots:
    void onViewSelectionChanged(QModelIndex index);
    void onTimerTimeout();
protected:
    void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;
private:
    Ui::FrontIoOverride *ui;

    QSqlRelationalTableModel* __inputTable;
    QSqlRelationalTableModel* __outputTable;

    QSqlRelationalTableModel* __signalTable;

    QSqlRelationalTableModel* __regionTable;

    ControllerManualMode* __controller;
    QTimer* __timer;
    MODBUS_U_WORD __currentViewIndex;
    QMap<QWidget*,SignalMonitor::WordOutBits> __labelAddressMap;

};

#endif // FRONTIOOVERRIDE_H
