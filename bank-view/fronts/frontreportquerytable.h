#ifndef FRONTREPORTQUERYTABLE_H
#define FRONTREPORTQUERYTABLE_H

#include <QWidget>
#include <QSqlTableModel>
#include <QSqlDatabase>
#include <QDateTime>
#include <QSqlError>

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
    void onAddPerformed();
    void onClearPerformed();
    void onExportPerformed();
protected:
    QSqlDatabase __db;

    QSqlTableModel* __queryTable;
    //!
    //! \brief __reportTable
    //! Read-only
    QSqlTableModel* __reportSingleTable;
    //!
    //! \brief __reportPollTable
    //!
    QSqlTableModel* __reportPollTable;

    QMap<TableNames,QSqlTableModel*> __referenceTables;
};

#endif // FRONTREPORTQUERYTABLE_H
