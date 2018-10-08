#include "frontiooverride.h"
#include "ui_frontiooverride.h"

FrontIoOverride::FrontIoOverride(QWidget *parent) :
    FrontCommon(parent),
    ui(new Ui::FrontIoOverride)
{
    ui->setupUi(this);
}


FrontIoOverride::~FrontIoOverride()
{
    delete ui;
}
//!
//! \brief FrontIoOverride::Setup
//! \param input
//! \param output
//! \param region
//!
void FrontIoOverride::Setup(QSqlTableModel *input,
                            QSqlTableModel *output,
                            QSqlTableModel *region,
                            QSqlTableModel *header)
{
    ui->tableViewInputs->setModel(input);
    HEADER_STRUCTURE::HeaderRender::renderViewHeader(header,ui->tableViewInputs);
    ui->widgetInputFilter->PrimaryTable(region);
    ui->widgetInputFilter->DataTable(input);

    ui->tableViewOutputs->setModel(output);
    HEADER_STRUCTURE::HeaderRender::renderViewHeader(header,ui->tableViewOutputs);
    ui->widgetOutputFilter->PrimaryTable(region);
    ui->widgetOutputFilter->DataTable(output);
}
