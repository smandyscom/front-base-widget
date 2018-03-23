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
    void onClicked();
    void onViewSelectionChanged();
private:
    Ui::FrontCylinderPanel *ui;

    AbstractQVariantSqlTable* __cylinderTable;

    ControllerManualMode* __controller;

    QList<TableModelCylinder::Offset> __inputFields;
    QList<TableModelCylinder::Offset> __outputFields;

    QString generateFilterString(QString key,QList<QString> conditions);
};

#endif // FRONTCYLINDERPANEL_H
