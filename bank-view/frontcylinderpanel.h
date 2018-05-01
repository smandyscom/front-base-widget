#ifndef FRONTCYLINDERPANEL_H
#define FRONTCYLINDERPANEL_H

#include <QWidget>
#include <abstractsqltableadpater.h>
#include <definitionbasicblocks.h>
#include <controllermanualmode.h>

#include <frontbanktransfer.h>
#include <frontsinglefilter.h>

#include <junctionbankdatabase.h>

#include <definitioncylinderblock.h>
#include <definitionioattributes.h>
#include <definitionauxiliarykeys.h>

#include <QSqlRecord>

namespace Ui {
class FrontCylinderPanel;
}

using namespace CylinderBlock;
using namespace IoAttributes;

class FrontCylinderPanel : public QWidget
{
    Q_OBJECT

public:
    explicit FrontCylinderPanel(QWidget *parent = 0);
    ~FrontCylinderPanel();
signals:
    void dataChanged(TransferTask task);
protected slots:
    void onCylinderCommandClicked();
    void onViewSelectionChanged();
    void onTimerTimeout();
protected:
    void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;
private:
    Ui::FrontCylinderPanel *ui;

    QSqlRelationalTableModel* __cylinderTable;
    QSqlRelationalTableModel* __headerTable;

    ControllerManualMode* __controller;

    QList<CylinderBlock::DataBaseHeaders> __inputFields;
    QList<CylinderBlock::DataBaseHeaders> __outputFields;

    QString generateFilterString(QString key,QList<QString> conditions);

    QTimer* __timer;

    MODBUS_U_WORD __currentViewIndex;

    QMap<QWidget*,CylinderMonitorBlock::Status> __labelAddressMap;
    QMap<QWidget*,Qt::GlobalColor> __labelColorMap;

    QList<QWidget*> __busyInterlock;
};

#endif // FRONTCYLINDERPANEL_H
