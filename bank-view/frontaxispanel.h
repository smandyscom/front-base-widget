#ifndef FRONTAXISPANEL_H
#define FRONTAXISPANEL_H

#include <QWidget>

#include <QSqlRelationalTableModel>

#include <frontsinglefilter.h>
#include <frontbanktransfer.h>

#include <definitionaxisblocks.h>
#include <utilities.h>

#include <junctionbankdatabase.h>

namespace Ui {
class FrontAxisPanel;
}

class FrontAxisPanel : public QWidget
{
    Q_OBJECT

public:
    explicit FrontAxisPanel(QWidget *parent = 0);
    ~FrontAxisPanel();

private:
    Ui::FrontAxisPanel *ui;
};

#endif // FRONTAXISPANEL_H
