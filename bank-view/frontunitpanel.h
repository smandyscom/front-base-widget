#ifndef FRONTUNITPANEL_H
#define FRONTUNITPANEL_H

#include <QWidget>
#include <QTimer>
#include <QPushButton>
#include <QSqlRelationalTableModel>

#include <frontsinglefilter.h>
#include <frontbanktransfer.h>

#include <controllermanualmode.h>

#include <definitionunitblocks.h>
#include <utilities.h>

#include <junctionbankdatabase.h>

#include <abstractauthreceiver.h>

namespace Ui {
class FrontUnitPanel;
}

typedef QPair<ModbusDriverAddress,Qt::GlobalColor> VisualAspect;

class FrontUnitPanel : public QWidget,
        public AbstractAuthReceiver
{
    Q_OBJECT

public:
    explicit FrontUnitPanel(QWidget *parent = 0);
    ~FrontUnitPanel();
protected slots:
    void onCommandClick();
    void onViewSelectionChanged();
    void onTimerTimeout();
protected:
    void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;
private:
    Ui::FrontUnitPanel *ui;

    QSqlRelationalTableModel* __unitTable;

    QTimer* __timer;
    MODBUS_U_WORD __currentViewIndex;

    ControllerManualMode* __controller;

    QMap<QWidget*,VisualAspect> __statusShowMap;
    QMap<QPushButton*,ModbusDriverAddress> __controlMap;//Widget,Address

    QList<QWidget*> __busyInterlock;
};

#endif // FRONTUNITPANEL_H
