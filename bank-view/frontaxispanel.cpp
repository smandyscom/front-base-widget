#include "frontaxispanel.h"
#include "ui_frontaxispanel.h"

FrontAxisPanel::FrontAxisPanel(QSqlRelationalTableModel *axisTable,
                               QSqlRelationalTableModel *regionTable,
                               QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrontAxisPanel)
{
    ui->setupUi(this);
    //!
    new FrontSingleFilter(axisTable,
                          regionTable,
                          utilities::trimNamespace(QVariant::fromValue(AxisBlock::REGION)),
                          utilities::trimNamespace(QVariant::fromValue(AxisBlock::NAME)),
                          ui->widgetFilter);
    new FrontBankTransfer(CommitBlock::SELECTION_AXIS,
                          ui->widgetTransfer);
    //!
    ui->tableViewAxis->setModel(axisTable);
}

FrontAxisPanel::~FrontAxisPanel()
{
    delete ui;
}
