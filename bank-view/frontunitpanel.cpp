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
    ui->tableViewUnit->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableViewUnit->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(ui->tableViewUnit,SIGNAL(clicked(QModelIndex)),this,SLOT(onViewSelectionChanged()));
    //!
    __controller = ControllerManualMode::Instance();
    //! Timer
    __timer = new QTimer(this);
    connect(__timer,SIGNAL(timeout()),this,SLOT(onTimerTimeout()));
    __timer->start();
    //!
    __statusShowMap[ui->labelCondition0] = VisualAspect(ModbusDriverAddress(0,UnitMonitorBlock::BIT_1_TEMP_CONDITION_1,UnitMonitorBlock::OFFSET_MONITOR_TRANSITION_WORD),Qt::green);
    __statusShowMap[ui->labelCondition1] = VisualAspect(ModbusDriverAddress(0,UnitMonitorBlock::BIT_2_TEMP_CONDITION_2,UnitMonitorBlock::OFFSET_MONITOR_TRANSITION_WORD),Qt::green);
    __statusShowMap[ui->labelCondition2] = VisualAspect(ModbusDriverAddress(0,UnitMonitorBlock::BIT_3_TEMP_CONDITION_3,UnitMonitorBlock::OFFSET_MONITOR_TRANSITION_WORD),Qt::green);
    __statusShowMap[ui->labelCondition3] = VisualAspect(ModbusDriverAddress(0,UnitMonitorBlock::BIT_4_TEMP_CONDITION_4,UnitMonitorBlock::OFFSET_MONITOR_TRANSITION_WORD),Qt::green);
    __statusShowMap[ui->labelCondition4] = VisualAspect(ModbusDriverAddress(0,UnitMonitorBlock::BIT_5_TEMP_CONDITION_5,UnitMonitorBlock::OFFSET_MONITOR_TRANSITION_WORD),Qt::green);
    __statusShowMap[ui->labelCondition5] = VisualAspect(ModbusDriverAddress(0,UnitMonitorBlock::BIT_6_TEMP_CONDITION_6,UnitMonitorBlock::OFFSET_MONITOR_TRANSITION_WORD),Qt::green);
    __statusShowMap[ui->labelCondition6] = VisualAspect(ModbusDriverAddress(0,UnitMonitorBlock::BIT_7_TEMP_CONDITION_7,UnitMonitorBlock::OFFSET_MONITOR_TRANSITION_WORD),Qt::green);
    __statusShowMap[ui->labelCondition7] = VisualAspect(ModbusDriverAddress(0,UnitMonitorBlock::BIT_8_TEMP_CONDITION_8,UnitMonitorBlock::OFFSET_MONITOR_TRANSITION_WORD),Qt::green);

    __statusShowMap[ui->pushButtonEnableStep] = VisualAspect(ModbusDriverAddress(UnitOperationBlock::OFFSET_UOB_EN_STEP),Qt::green);
    __statusShowMap[ui->pushButtonStep] = VisualAspect(ModbusDriverAddress(UnitOperationBlock::OFFSET_UOB_TRIG_STEP),Qt::green);
    __statusShowMap[ui->pushButtonWorking] = VisualAspect(ModbusDriverAddress(UnitOperationBlock::OFFSET_UOB_WORKING_OVERRIDE),Qt::green);
    __statusShowMap[ui->pushButtonPause] = VisualAspect(ModbusDriverAddress(UnitOperationBlock::OFFSET_UOB_STATE_PAUSE),Qt::yellow);
    //!
    __controlMap[ui->pushButtonEnableStep] = ModbusDriverAddress(UnitOperationBlock::OFFSET_UOB_EN_STEP);
    __controlMap[ui->pushButtonStep] = ModbusDriverAddress(UnitOperationBlock::OFFSET_UOB_TRIG_STEP);
    __controlMap[ui->pushButtonWorking] = ModbusDriverAddress(UnitOperationBlock::OFFSET_UOB_WORKING_OVERRIDE);
    __controlMap[ui->pushButtonPause] = ModbusDriverAddress(UnitOperationBlock::OFFSET_UOB_STATE_PAUSE);
    foreach (QPushButton* var, __controlMap.keys()) {
        connect(var,SIGNAL(clicked(bool)),this,SLOT(onCommandClick()));
    }
    //! Interlock
    __busyInterlock = {
        ui->pushButtonEnableStep,
        ui->pushButtonStep,
        ui->pushButtonWorking,
        ui->pushButtonPause
    };
}

FrontUnitPanel::~FrontUnitPanel()
{
    delete ui;
}

void FrontUnitPanel::onViewSelectionChanged()
{
    QSqlRecord __record = __unitTable->record(ui->tableViewUnit->selectionModel()->selectedRows().first().row());
    //! Changeover monitor index
    __currentViewIndex =  __record.value(QVariant::fromValue(UnitBlock::ID).toString()).value<MODBUS_S_WORD>();
    __controller->onMonitorDeviceIndexChanged(__currentViewIndex);
}
void FrontUnitPanel::onTimerTimeout()
{
    //!
    UnitMonitorBlock umb;
    *static_cast<CellDataBlock*>(&umb) = __controller->MonitorBlock();
    //! Bit
    foreach (QWidget* var, __statusShowMap.keys()) {
        utilities::colorChangeOver(var,
                                   umb.Value(__statusShowMap[var].first.getAddress()).toBool(),
                                   __statusShowMap[var].second);
    }
    //!Numeric
    ui->textBrowserState->setText(QString("0x%1").arg(QString::number(umb.Value(UnitMonitorBlock::OFFSET_MONITOR_STATE).toUInt(),16)));
    ui->textBrowserNextState->setText(QString("0x%1").arg(QString::number(umb.Value(UnitMonitorBlock::OFFSET_MONITOR_NEXT_STATE).toUInt(),16)));
    ui->textBrowserTickTime->setText(umb.Value(UnitMonitorBlock::OFFSET_MONITOR_WORKING_TIMER_CACHE).toString());
    //! Interlock
    foreach (QWidget* var, __busyInterlock) {
        var->setEnabled(__controller->CurrentState()==ControllerManualMode::STATE_IDLE);
    }
}

void FrontUnitPanel::onCommandClick()
{
    QPushButton* __button = qobject_cast<QPushButton*>(sender());

    UnitOperationBlock uob;
    //!Fetch last status
    UnitMonitorBlock umb;
    *static_cast<CellDataBlock*>(&umb) = __controller->MonitorBlock();
    //! Flip
    uob.Value(__controlMap[__button].getAddress(),
            QVariant::fromValue(!umb.Value(__controlMap[__button].getAddress()).toBool()));
    //!
    CommitBlock __cb;
    __cb.Mode(CommitBlock::MODE_EXE_UNIT);
    __controller->CommitOption(__cb);
    __controller->DataBlock(QVariant::fromValue(static_cast<CellDataBlock>(uob)));
    emit __controller->operationTriggered();
}

void FrontUnitPanel::showEvent(QShowEvent *event)
{
    //! Set monitoring focus
    __controller->MonitorDeviceCategrory(CommitBlock::SELECTION_UNIT);
    __controller->onMonitorDeviceIndexChanged(__currentViewIndex);
    //!
    QWidget::showEvent(event);
}
