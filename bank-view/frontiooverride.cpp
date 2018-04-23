#include "frontiooverride.h"
#include "ui_frontiooverride.h"

FrontIoOverride::FrontIoOverride(QSqlRelationalTableModel *inputTable,
                                 QSqlRelationalTableModel *outputTable,
                                 QSqlRelationalTableModel *regionTable,
                                 QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrontIoOverride),
    __inputTable(new TableModelIOOverride(inputTable)),
    __outputTable(new TableModelIOOverride(outputTable)),
    __regionTable(regionTable)
{
    ui->setupUi(this);
    //!
    new FrontSingleFilter(inputTable,
                          regionTable,
                          utilities::trimNamespace(QVariant::fromValue(TableModelIOOverride::REGION)),
                          utilities::trimNamespace(QVariant::fromValue(TableModelIOOverride::NAME)),
                          ui->widgetInputFilter);
    new FrontSingleFilter(outputTable,
                          regionTable,
                          utilities::trimNamespace(QVariant::fromValue(TableModelIOOverride::REGION)),
                          utilities::trimNamespace(QVariant::fromValue(TableModelIOOverride::NAME)),
                          ui->widgetOutputFilter);
    //!
    QList<QTableView*> __views = {ui->tableViewInputs,
                                 ui->tableViewOutputs};

    ui->tableViewInputs->setModel(inputTable);
    ui->tableViewOutputs->setModel(outputTable);
    foreach (QTableView* view, __views) {
        view->hideColumn(TableModelIOOverride::ID);
        view->hideColumn(TableModelIOOverride::HAL_ADDRESS);
        view->hideColumn(TableModelIOOverride::REGION);
        view->setCornerButtonEnabled(false);
    }
    //!
    connect(ui->tableViewOutputs,&QTableView::clicked,[=](QModelIndex index){
        ModbusDriverAddress address = ModbusDriverAddress(__outputTable->index(index.row(),TableModelIOOverride::HAL_ADDRESS).data().toInt());
        ModbusChannel::Instance()->Access<bool>(address,!ModbusChannel::Instance()->Access<bool>(address));
    });
}


FrontIoOverride::~FrontIoOverride()
{
    delete ui;
}
