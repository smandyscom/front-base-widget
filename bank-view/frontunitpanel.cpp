#include "frontunitpanel.h"
#include "ui_frontunitpanel.h"

FrontUnitPanel::FrontUnitPanel(QSqlRelationalTableModel *unitTable, QSqlRelationalTableModel *regionTable, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrontUnitPanel)
{
    ui->setupUi(this);
    //!
    new FrontSingleFilter(unitTable,
                          regionTable,
                          utilities::trimNamespace(QVariant::fromValue(UnitBlock::LSID_BASE)),
                          utilities::trimNamespace(QVariant::fromValue(UnitBlock::NAME)),
                          ui->widgetFilter);
    new FrontBankTransfer(CommitBlock::SELECTION_UNIT,
                          ui->widgetTransfer);
    //!
    ui->tableViewUnit->setModel(unitTable);
}

FrontUnitPanel::~FrontUnitPanel()
{
    delete ui;
}
