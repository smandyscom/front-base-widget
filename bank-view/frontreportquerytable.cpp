#include "frontreportquerytable.h"
#include "ui_frontreportquerytable.h"

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
