#include "frontreportquerytable.h"
#include "ui_frontreportquerytable.h"
#include <QDebug>
FrontReportQueryTable::FrontReportQueryTable(QSqlDatabase db,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrontReportQueryTable),
    __db(db)
{
    ui->setupUi(this);
    //!
    __queryTable = new QSqlTableModel(this,__db);
    __queryTable->setTable(QVariant::fromValue(QUERY_HEAD).toString());
    __queryTable->setEditStrategy(QSqlTableModel::OnManualSubmit);
    __queryTable->select();
    //!
    __reportTable = new QSqlTableModel(this,__db);
    __reportTable->setTable(QVariant::fromValue(VIEW_REPORT_OVERVIEW).toString());
    __reportTable->select();
    //!
    QList<TableNames> __names = {VIEW_MAT_TABLE_HOUSING,
                                VIEW_DISTINCT_WORKING_NUMBER,
                                VIEW_DISTINCT_SHIFT,
                                VIEW_DISTINCT_EID,
                                VIEW_DISTINCT_DATE};
    foreach(TableNames var,__names)
    {
        QSqlTableModel* __table = new QSqlTableModel(this,__db);
        __table->setTable(QVariant::fromValue(var).toString());
        if(!__table->select())
            qDebug() << QString("Table not found:%1").arg(__table->tableName());
        __referenceTables[var] = __table;
    }

    //!
    QSqlTableModel* __header = new QSqlTableModel(this,__db);
    __header->setTable(QVariant::fromValue(HEADER_REPORT_OVERVIEW).toString());
    __header->select();
    ui->tableViewReport->setModel(__reportTable);
    HEADER_STRUCTURE::HeaderRender::renderViewHeader(__header,ui->tableViewReport);


    __header->setTable(QVariant::fromValue(HEADER_REPORT_HEAD).toString());
    __header->select();
    ui->tableViewQuery->setModel(__queryTable);
    HEADER_STRUCTURE::HeaderRender::renderViewHeader(__header,ui->tableViewQuery);
    //!
    connect(ui->pushButtonQuery,SIGNAL(clicked(bool)),this,SLOT(onQueryPerformed()));
    connect(ui->pushButtonExport,SIGNAL(clicked(bool)),this,SLOT(onExportPerformed()));

    //!
    ui->tableViewQuery->setItemDelegateForRow(HOUSING1,
                                              new DelegateMaterialSelector(__referenceTables[VIEW_MAT_TABLE_HOUSING],VALUE, ui->tableViewQuery));

    ui->tableViewQuery->setItemDelegateForRow(HOUSING2,
                                              new DelegateMaterialSelector(__referenceTables[VIEW_MAT_TABLE_HOUSING],VALUE, ui->tableViewQuery));

    ui->tableViewQuery->setItemDelegateForRow(WORKINGNUMBER,
                                              new DelegateMaterialSelector(__referenceTables[VIEW_DISTINCT_WORKING_NUMBER],VALUE, ui->tableViewQuery));

    ui->tableViewQuery->setItemDelegateForRow(SHIFT,
                                              new DelegateMaterialSelector(__referenceTables[VIEW_DISTINCT_SHIFT],VALUE, ui->tableViewQuery));

    ui->tableViewQuery->setItemDelegateForRow(EID,
                                              new DelegateMaterialSelector(__referenceTables[VIEW_DISTINCT_EID],VALUE, ui->tableViewQuery));

    ui->tableViewQuery->setItemDelegateForRow(REPORTDATE,
                                              new DelegateMaterialSelector(__referenceTables[VIEW_DISTINCT_DATE],VALUE, ui->tableViewQuery));
}

FrontReportQueryTable::~FrontReportQueryTable()
{
    delete ui;
}

void FrontReportQueryTable::onQueryPerformed()
{
    __queryTable->submitAll();
    __reportTable->select(); //reselect-renew
}

void FrontReportQueryTable::onExportPerformed()
{

}
