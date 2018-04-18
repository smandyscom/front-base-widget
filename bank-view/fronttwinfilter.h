#ifndef FRONTWINFILTER_H
#define FRONTWINFILTER_H

#include <QWidget>
#include <QList>
#include <QTableView>
#include <QMap>

#include <abstractsqltableadpater.h>
#include <QSqlRelationalTableModel>
namespace Ui {
class FrontTwinFilter;
}

class FrontTwinFilter : public QWidget
{
    Q_OBJECT

public:
    explicit FrontTwinFilter(QSqlRelationalTableModel* dataTable,
                             QVariant key1,
                             QSqlRelationalTableModel* primaryTable,
                             QVariant key2,
                             QSqlRelationalTableModel* secondaryTable,
                             QWidget *parent = 0);
    ~FrontTwinFilter();

signals:
    void primarySelected(QVariant primaryKey);
protected slots:
    void onSelectionChanged(int i);
    void onSelectAll();
private:
    Ui::FrontTwinFilter *ui;

    QSqlRelationalTableModel* __dataTable;
    QString __key1;
    //!
    //! \brief __primaryTable
    //! Load to
    QSqlRelationalTableModel* __primaryTable;
    QString __key2;
    //!
    //! \brief __secondaryTable
    //!
    QSqlRelationalTableModel* __secondaryTable;

    //!
    QMap<QWidget*,QSqlRelationalTableModel*> __manipulateMap;
};

#endif // FRONTWINFILTER_H