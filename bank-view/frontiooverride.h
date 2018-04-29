#ifndef FRONTIOOVERRIDE_H
#define FRONTIOOVERRIDE_H

#include <QWidget>
#include <QSqlRelationalTableModel>
#include <tablemodeliooverride.h>
#include <modbuschannel.h>

#include <frontsinglefilter.h>
#include <junctionbankdatabase.h>

#include <utilities.h>

namespace Ui {
class FrontIoOverride;
}



class FrontIoOverride : public QWidget
{
    Q_OBJECT

public:
    explicit FrontIoOverride(QWidget *parent = 0);
    ~FrontIoOverride();
private:
    Ui::FrontIoOverride *ui;

    QSqlRelationalTableModel* __inputTable;
    QSqlRelationalTableModel* __outputTable;
    QSqlRelationalTableModel* __regionTable;
};

#endif // FRONTIOOVERRIDE_H
