#include "frontiooverride.h"
#include "ui_frontiooverride.h"

FrontIoOverride::FrontIoOverride(QSqlRelationalTableModel *inputTable,
                                 QSqlRelationalTableModel *outputTable,
                                 QSqlRelationalTableModel *regionTable,
                                 QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrontIoOverride),
    __inputTable(inputTable),
    __outputTable(outputTable),
    __regionTable(regionTable)
{
    ui->setupUi(this);

    //!
    QList<QComboBox*> __comboxes = {ui->comboBoxInputsFilter,
                                    ui->comboBoxOutputsFilter};
    //!
    foreach (QComboBox* var, __comboxes) {
        var->setView(new QTableView(var));
        var->setModel(regionTable);
        var->setModelColumn(1);
        connect(var,SIGNAL(currentIndexChanged(int)),this,SLOT(onComboxCurrentIndexChanged()));
    }
    //!
    QList<QPushButton*> __buttons = {ui->pushButtonInputsSelectAllRegion,
                                     ui->pushButtonOutputsSelectAllRegion};
    foreach (QPushButton* var, __buttons) {
        connect(var,&QPushButton::clicked,this,&FrontIoOverride::onSelectAll);
    }
    //!
    QList<QTableView*> __views = {ui->tableViewInputs,
                                 ui->tableViewOutputs};

    ui->tableViewInputs->setModel(inputTable);
    ui->tableViewOutputs->setModel(outputTable);
    foreach (QTableView* view, __views) {
        view->hideColumn(TableModelIOOverride::ID);
        view->hideColumn(TableModelIOOverride::HAL_ADDRESS);
        view->hideColumn(TableModelIOOverride::REGION);
    }
}
//!
//! \brief FrontIoOverride::onComboxCurrentIndexChanged
//!
void FrontIoOverride::onComboxCurrentIndexChanged()
{
    QComboBox* cb = qobject_cast<QComboBox*>(sender());
    QString regionKey = QVariant::fromValue(TableModelIOOverride::REGION).value<QString>();
    int regionId = __regionTable->index(cb->currentIndex(),0).data().toUInt();

    QString filterString = QString("%1=%2").arg(regionKey,regionId);

    if(sender()==ui->comboBoxInputsFilter)
    {
        __inputTable->setFilter(filterString);
    }
    else if(sender()==ui->comboBoxOutputsFilter)
    {
        __outputTable->setFilter(filterString);
    }
}
//!
//! \brief FrontIoOverride::onSelectAll
//!
void FrontIoOverride::onSelectAll()
{
    if(sender()==ui->pushButtonInputsSelectAllRegion)
    {
        __inputTable->setFilter("");
    }
    else if(sender()==ui->pushButtonOutputsSelectAllRegion)
    {
        __outputTable->setFilter("");
    }
}


FrontIoOverride::~FrontIoOverride()
{
    delete ui;
}
