#ifndef FRONTCYLINDERPANEL_H
#define FRONTCYLINDERPANEL_H

#include <QWidget>
#include <abstractqvariantsqltable.h>
#include <basicblocksdefinition.h>
namespace Ui {
class FrontCylinderPanel;
}

class FrontCylinderPanel : public QWidget
{
    Q_OBJECT

public:
    explicit FrontCylinderPanel(AbstractQVariantSqlTable* cylinderTable,
                                QWidget *parent = 0);
    ~FrontCylinderPanel();
protected slots:
    void onClicked();

private:
    Ui::FrontCylinderPanel *ui;

    AbstractQVariantSqlTable* __cylinderTable;
};

#endif // FRONTCYLINDERPANEL_H
