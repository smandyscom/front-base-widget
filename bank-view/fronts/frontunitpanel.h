#ifndef FRONTUNITPANEL_H
#define FRONTUNITPANEL_H

#include <frontcommonmanual.h>
#include <definitionmanualblock.h>
#include <definitionunitblocks.h>
#include <utilities.h>

//#include <abstractauthreceiver.h>

namespace Ui {
class FrontUnitPanel;
}

//typedef QPair<ModbusDriverAddress,Qt::GlobalColor> VisualAspect;

class FrontUnitPanel : public FrontCommonManual
//        public AbstractAuthReceiver
{
    Q_OBJECT

public:
    explicit FrontUnitPanel(QWidget *parent = 0);
    ~FrontUnitPanel();

    void Setup(QSqlTableModel* unitTable, QSqlTableModel *unitTableHeader);

protected slots:
    void onCommandClick();
//    void onViewSelectionChanged();
//    void onTimerTimeout();
protected:
//    void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;
private:
    Ui::FrontUnitPanel *ui;

//    QSqlTableModel* __unitTable;

//    QTimer* __timer;
//    MODBUS_U_WORD __currentViewIndex;

//    ControllerManualMode* __controller;

//    QMap<QWidget*,VisualAspect> __statusShowMap;
    QMap<QPushButton*,UnitOperationBlock::ControlBits> m_controlMap;//Widget,Address

    UnitMonitorBlock m_monitor;
//    QList<QWidget*> __busyInterlock;

    //!Override
    int currentIndex() const Q_DECL_OVERRIDE;
};

#endif // FRONTUNITPANEL_H
