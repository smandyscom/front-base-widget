#ifndef FRONTCYLINDERPANEL_H
#define FRONTCYLINDERPANEL_H

#include <frontcommonmanual.h>
#include <abstractsqltableadpater.h>
#include <definitionmanualblock.h>

#include <definitioncylinderblock.h>
#include <definitionioattributes.h>
#include <definitionauxiliarykeys.h>

//#include <abstractauthreceiver.h>

namespace Ui {
class FrontCylinderPanel;
}

using namespace CylinderBlock;
using namespace IoAttributes;

class FrontCylinderPanel :
        public FrontCommonManual
{
    Q_OBJECT

public:
    explicit FrontCylinderPanel(QWidget *parent = nullptr);
    ~FrontCylinderPanel();

    void Setup(QSqlTableModel* cylinderTable,
               QSqlTableModel* regionTable,
               QSqlTableModel* cylinderTableHeader);

//signals:
//    void dataChanged(TransferTask task);
protected slots:
    void onCylinderCommandClicked();
//    void onViewSelectionChanged(QModelIndex index);
//    void onTimerTimeout();
//protected:
//    void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;
private:
    Ui::FrontCylinderPanel *ui;

//    QSqlTableModel* __cylinderTable;
//    QSqlTableModel* __headerTable;

//    ControllerManualMode* __controller;

//    QTimer* __timer;

//    MODBUS_U_WORD __currentViewIndex;

//    QMap<QWidget*,CylinderMonitorBlock::Status> __labelAddressMap;
//    QMap<QWidget*,Qt::GlobalColor> __labelColorMap;

//    QList<QWidget*> __busyInterlock;


};

#endif // FRONTCYLINDERPANEL_H
