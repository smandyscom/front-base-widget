#include "frontiooverride.h"
#include "ui_frontiooverride.h"

FrontIoOverride::FrontIoOverride(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrontIoOverride),
    __inputTable(new TableModelIOOverride(JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::WHOLE_INPUTS))),
    __outputTable(new TableModelIOOverride(JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::WHOLE_OUTPUTS))),
    __regionTable(JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::DEF_REGION))
{
    ui->setupUi(this);
    //!
    FrontSingleFilter* __fsf = new FrontSingleFilter(ui->widgetInputFilter);
    __fsf->PrimaryTable(__regionTable);
    __fsf->DataTable(__inputTable);
    //!
    __fsf = new FrontSingleFilter(ui->widgetOutputFilter);
    __fsf->PrimaryTable(__regionTable);
    __fsf->DataTable(__outputTable);
    //!
    QList<QTableView*> __views = {ui->tableViewInputs,
                                 ui->tableViewOutputs};

    ui->tableViewInputs->setModel(__inputTable);
    ui->tableViewOutputs->setModel(__outputTable);
    HEADER_STRUCTURE::HeaderRender::renderViewHeader(JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::HEADER_IO),
                                                     ui->tableViewInputs);
    HEADER_STRUCTURE::HeaderRender::renderViewHeader(JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::HEADER_IO),
                                                     ui->tableViewOutputs);
    //!
    connect(ui->tableViewOutputs,&QTableView::clicked,[=](QModelIndex index){
        ModbusDriverAddress address(__outputTable->record(index.row())
                                    .value(utilities::trimNamespace(QVariant::fromValue(TableModelIOOverride::HAL_ADDRESS))).toUInt());
        ModbusChannel::Instance()->Access<bool>(address,!ModbusChannel::Instance()->Access<bool>(address));
    });
}


FrontIoOverride::~FrontIoOverride()
{
    delete ui;
}
