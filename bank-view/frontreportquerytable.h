#ifndef FRONTREPORTQUERYTABLE_H
#define FRONTREPORTQUERYTABLE_H

#include <QWidget>
#include <QSqlTableModel>
#include <QSqlDatabase>

#include <definitionauxiliarykeys.h>
#include <delegatematerialselector.h>
#include <definitionmaterialkeys.h>
using namespace MaterialKeys;

namespace Ui {
class FrontReportQueryTable;
}

class FrontReportQueryTable : public QWidget
{
    Q_OBJECT

public:
    explicit FrontReportQueryTable(QSqlDatabase db,QWidget *parent = 0);
    ~FrontReportQueryTable();

private:
    Ui::FrontReportQueryTable *ui;
protected slots:
    void onQueryPerformed();
    void onExportPerformed();
protected:
    QSqlDatabase __db;

    QSqlTableModel* __queryTable;
    //!
    //! \brief __reportTable
    //! Read-only
    QSqlTableModel* __reportTable;

    QMap<TableNames,QSqlTableModel*> __referenceTables;
};

#endif // FRONTREPORTQUERYTABLE_H
