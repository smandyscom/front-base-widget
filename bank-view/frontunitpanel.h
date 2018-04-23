#ifndef FRONTUNITPANEL_H
#define FRONTUNITPANEL_H

#include <QWidget>

#include <QSqlRelationalTableModel>

#include <frontsinglefilter.h>
#include <frontbanktransfer.h>

#include <definitionunitblocks.h>
#include <utilities.h>

namespace Ui {
class FrontUnitPanel;
}

class FrontUnitPanel : public QWidget
{
    Q_OBJECT

public:
    explicit FrontUnitPanel(QSqlRelationalTableModel *axisTable,
                            QSqlRelationalTableModel *regionTable,
                            QWidget *parent = 0);
    ~FrontUnitPanel();

private:
    Ui::FrontUnitPanel *ui;
};

#endif // FRONTUNITPANEL_H
