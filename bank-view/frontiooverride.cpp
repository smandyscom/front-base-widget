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
        QTableView* qtv = new QTableView(var);
        var->setView(qtv);
        var->setModel(regionTable);
        var->setModelColumn(1);
        qtv->hideColumn(0);
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
        view->setCornerButtonEnabled(false);
    }
    //!
    connect(ui->tableViewOutputs,&QTableView::clicked,[=](QModelIndex index){
        ModbusDriverAddress address = ModbusDriverAddress(__outputTable->index(index.row(),TableModelIOOverride::HAL_ADDRESS).data().toInt());
        ModbusChannel::Instance()->Access<bool>(address,!ModbusChannel::Instance()->Access<bool>(address));
    });
}
//!
//! \brief FrontIoOverride::onComboxCurrentIndexChanged
//!
void FrontIoOverride::onComboxCurrentIndexChanged()
{
    QComboBox* cb = qobject_cast<QComboBox*>(sender());
    QString regionKey = QVariant::fromValue(TableModelIOOverride::REGION).value<QString>();
    uint regionId = __regionTable->index(cb->currentIndex(),0).data().toUInt();

    QString filterString = tr("%1=%2").arg(regionKey).arg(regionId);
    //QString filterString = tr("%1=%2").arg("PLC_ADDRESS").arg("\'803E\'");

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
        __inputTable->select();
    }
    else if(sender()==ui->pushButtonOutputsSelectAllRegion)
    {
        __outputTable->setFilter("");
        __outputTable->select();
    }
}


FrontIoOverride::~FrontIoOverride()
{
    delete ui;
}
