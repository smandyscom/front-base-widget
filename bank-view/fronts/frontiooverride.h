#ifndef FRONTIOOVERRIDE_H
#define FRONTIOOVERRIDE_H

#include <QWidget>
#include <QSqlTableModel>

#include <definitionauxiliarykeys.h>
//#include <modbuschannel.h>

//#include <frontsinglefilter.h>
//#include <junctionbankdatabase.h>

#include <utilities.h>

//#include <definitionsignalblock.h>
//#include <controllermanualmode.h>

//#include <abstractauthreceiver.h>

namespace Ui {
class FrontIoOverride;
}



class FrontIoOverride : public QWidget /*,
        public AbstractAuthReceiver*/
{
    Q_OBJECT

public:
    explicit FrontIoOverride(QWidget *parent = 0);
    ~FrontIoOverride();

    void Setup(QSqlTableModel* input,
               QSqlTableModel* output,
               QSqlTableModel* region,
               QSqlTableModel *header);

protected slots:
    void onViewSelectionChanged(QModelIndex index);
    void onTimerTimeout();
    void onOverrideClicked(QModelIndex index);
protected:
//    void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;
private:
    Ui::FrontIoOverride *ui;

//    QSqlTableModel* __inputTable;
//    QSqlTableModel* __outputTable;

//    QSqlTableModel* __signalTable;

//    QSqlTableModel* __regionTable;

//    ControllerManualMode* __controller;
//    QTimer* __timer;
//    MODBUS_U_WORD __currentViewIndex;
//    QMap<QWidget*,SignalMonitor::WordOutBits> __labelAddressMap;

};

#endif // FRONTIOOVERRIDE_H
