#ifndef FRONTREPORTQUERYTABLE_H
#define FRONTREPORTQUERYTABLE_H

#include <QWidget>
#include <QSqlTableModel>
#include <QSqlDatabase>

#include <definitionauxiliarykeys.h>

namespace Ui {
class FrontReportQueryTable;
}

class FrontReportQueryTable : public QWidget
{
    Q_OBJECT

public:
    enum TableName
    {
        QUERY_HEAD,
        VIEW_REPORT_OVERVIEW,
        //!
        HEADER_REPORT_OVERVIEW,
        HEADER_REPORT_HEAD,
    };
    Q_ENUM(TableName)

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
};

#endif // FRONTREPORTQUERYTABLE_H
