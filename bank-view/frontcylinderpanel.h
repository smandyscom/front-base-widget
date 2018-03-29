#ifndef FRONTCYLINDERPANEL_H
#define FRONTCYLINDERPANEL_H

#include <QWidget>
#include <abstractqvariantsqltable.h>
#include <basicblocksdefinition.h>
#include <controllermanualmode.h>

#include <frontbanktransfer.h>
#include <frontiooverride.h>
#include <frontsinglefilter.h>

#include <junctionbankdatabase.h>

#include <cylinderblockdefinition.h>

namespace Ui {
class FrontCylinderPanel;
}

class FrontCylinderPanel : public QWidget
{
    Q_OBJECT

public:
    explicit FrontCylinderPanel(QWidget *parent = 0);
    ~FrontCylinderPanel();
protected slots:
    void onCylinderCommandClicked();
    void onViewSelectionChanged();
    //!
    //! \brief onTimerTimeout
    //!
    void onTimerTimeout();
protected:
    void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;
private:
    Ui::FrontCylinderPanel *ui;

    AbstractQVariantSqlTable* __cylinderTable;

    ControllerManualMode* __controller;

    QList<TableModelCylinder::Offset> __inputFields;
    QList<TableModelCylinder::Offset> __outputFields;

    QString generateFilterString(QString key,QList<QString> conditions);

    QTimer* __timer;

    MODBUS_WORD __currentViewIndex;

};

#endif // FRONTCYLINDERPANEL_H
