#ifndef FRONTIOOVERRIDE_H
#define FRONTIOOVERRIDE_H

#include <QWidget>
#include <QSqlRelationalTableModel>
#include <tablemodeliooverride.h>
#include <modbuschannel.h>
namespace Ui {
class FrontIoOverride;
}

class FrontIoOverride : public QWidget
{
    Q_OBJECT

public:
    explicit FrontIoOverride(QSqlRelationalTableModel *inputTable,
                             QSqlRelationalTableModel *outputTable,
                             QSqlRelationalTableModel *regionTable,
                             QWidget *parent = 0);
    ~FrontIoOverride();
protected slots:
    void onComboxCurrentIndexChanged();
    void onSelectAll();
private:
    Ui::FrontIoOverride *ui;

    QSqlRelationalTableModel* __inputTable;
    QSqlRelationalTableModel* __outputTable;
    QSqlRelationalTableModel* __regionTable;
};

#endif // FRONTIOOVERRIDE_H
