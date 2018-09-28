#include "frontunitpanel.h"
#include "ui_frontunitpanel.h"

FrontUnitPanel::FrontUnitPanel(QWidget *parent) :
    FrontCommonManual(parent),
    ui(new Ui::FrontUnitPanel)
//    __unitTable(JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::WHOLE_UNITS))
{
    ui->setupUi(this);
    //!
    //FrontSingleFilter* __fsf = new FrontSingleFilter(ui->widgetFilter);
    //__fsf->DataTable(__unitTable);
    //__fsf->PrimaryTable(JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::DEF_REGION));
    //!
        //!
//    __controller = ControllerManualMode::Instance();
//    //! Timer
//    __timer = new QTimer(this);
//    connect(__timer,SIGNAL(timeout()),this,SLOT(onTimerTimeout()));
//    __timer->start();
    //!
//    __statusShowMap[ui->labelCondition0] = VisualAspect(ModbusDriverAddress(0,UnitMonitorBlock::BIT_1_TEMP_CONDITION_1,UnitMonitorBlock::OFFSET_MONITOR_TRANSITION_WORD),Qt::green);
//    __statusShowMap[ui->labelCondition1] = VisualAspect(ModbusDriverAddress(0,UnitMonitorBlock::BIT_2_TEMP_CONDITION_2,UnitMonitorBlock::OFFSET_MONITOR_TRANSITION_WORD),Qt::green);
//    __statusShowMap[ui->labelCondition2] = VisualAspect(ModbusDriverAddress(0,UnitMonitorBlock::BIT_3_TEMP_CONDITION_3,UnitMonitorBlock::OFFSET_MONITOR_TRANSITION_WORD),Qt::green);
//    __statusShowMap[ui->labelCondition3] = VisualAspect(ModbusDriverAddress(0,UnitMonitorBlock::BIT_4_TEMP_CONDITION_4,UnitMonitorBlock::OFFSET_MONITOR_TRANSITION_WORD),Qt::green);
//    __statusShowMap[ui->labelCondition4] = VisualAspect(ModbusDriverAddress(0,UnitMonitorBlock::BIT_5_TEMP_CONDITION_5,UnitMonitorBlock::OFFSET_MONITOR_TRANSITION_WORD),Qt::green);
//    __statusShowMap[ui->labelCondition5] = VisualAspect(ModbusDriverAddress(0,UnitMonitorBlock::BIT_6_TEMP_CONDITION_6,UnitMonitorBlock::OFFSET_MONITOR_TRANSITION_WORD),Qt::green);
//    __statusShowMap[ui->labelCondition6] = VisualAspect(ModbusDriverAddress(0,UnitMonitorBlock::BIT_7_TEMP_CONDITION_7,UnitMonitorBlock::OFFSET_MONITOR_TRANSITION_WORD),Qt::green);
//    __statusShowMap[ui->labelCondition7] = VisualAspect(ModbusDriverAddress(0,UnitMonitorBlock::BIT_8_TEMP_CONDITION_8,UnitMonitorBlock::OFFSET_MONITOR_TRANSITION_WORD),Qt::green);

//    __statusShowMap[ui->pushButtonEnableStep] = VisualAspect(ModbusDriverAddress(UnitOperationBlock::OFFSET_UOB_EN_STEP),Qt::magenta);
//    __statusShowMap[ui->pushButtonStep] = VisualAspect(ModbusDriverAddress(UnitOperationBlock::OFFSET_UOB_TRIG_STEP),Qt::green);
//    __statusShowMap[ui->pushButtonWorking] = VisualAspect(ModbusDriverAddress(UnitOperationBlock::OFFSET_UOB_WORKING_OVERRIDE),Qt::cyan);
//    __statusShowMap[ui->pushButtonPause] = VisualAspect(ModbusDriverAddress(UnitOperationBlock::OFFSET_UOB_STATE_PAUSE),Qt::yellow);
//    __statusShowMap[ui->pushButtonMaterial] = VisualAspect(ModbusDriverAddress(UnitOperationBlock::OFFSET_UOB_MATERIAL_OVERRIDE),Qt::green);
//    __statusShowMap[ui->pushButtonByPass] = VisualAspect(ModbusDriverAddress(UnitOperationBlock::OFFSET_UOB_IS_BYPASSED),Qt::blue);

    //!
    m_controlMap[ui->pushButtonEnableStep] = (UnitOperationBlock::OFFSET_UOB_EN_STEP);
    m_controlMap[ui->pushButtonStep] = (UnitOperationBlock::OFFSET_UOB_TRIG_STEP);
    m_controlMap[ui->pushButtonWorking] = (UnitOperationBlock::OFFSET_UOB_WORKING_OVERRIDE);
    m_controlMap[ui->pushButtonPause] = (UnitOperationBlock::OFFSET_UOB_STATE_PAUSE);
    m_controlMap[ui->pushButtonMaterial] = (UnitOperationBlock::OFFSET_UOB_MATERIAL_OVERRIDE);
    m_controlMap[ui->pushButtonByPass] = (UnitOperationBlock::OFFSET_UOB_IS_BYPASSED);

    foreach (QPushButton* var, m_controlMap.keys()) {
        connect(var,SIGNAL(clicked(bool)),this,SLOT(onCommandClick()));
    }
    //! Interlock
//    __busyInterlock = {
//        ui->pushButtonEnableStep,
//        ui->pushButtonStep,
//        ui->pushButtonWorking,
//        ui->pushButtonPause,
//        ui->pushButtonMaterial
//    };

//    //!
//    __disableList[AUTH::ROLE_OPERATOR].append(this);
}

FrontUnitPanel::~FrontUnitPanel()
{
    delete ui;
}

void FrontUnitPanel::Setup(QSqlTableModel* unitTable,
                           QSqlTableModel* unitTableHeader)
{
    mainDataTable = unitTable;
    //
    ui->tableViewUnit->setModel(unitTable);
    connect(ui->tableViewUnit,&QTableView::clicked,this,&FrontUnitPanel::onMonitorIndexChanged);
    //!
    HEADER_STRUCTURE::HeaderRender::renderViewHeader(unitTableHeader,
                                                     ui->tableViewUnit);
}

//void FrontUnitPanel::onTimerTimeout()
//{
//    //!
//    UnitMonitorBlock umb;
//    *static_cast<CellDataBlock*>(&umb) = __controller->MonitorBlock();
//    //! Bit
//    foreach (QWidget* var, __statusShowMap.keys()) {
//        utilities::colorChangeOver(var,
//                                   umb.Value(__statusShowMap[var].first.getAddress()).toBool(),
//                                   __statusShowMap[var].second);
//    }
//    //!Numeric
//    ui->lcdNumberState->display(umb.Value(UnitMonitorBlock::OFFSET_MONITOR_STATE).toInt());
//    ui->lcdNumberNextState->display(umb.Value(UnitMonitorBlock::OFFSET_MONITOR_NEXT_STATE).toInt());
//    ui->lcdNumberCycleTime->display(umb.Value(UnitMonitorBlock::OFFSET_MONITOR_WORKING_TIMER_CACHE).toReal());
//    //! Interlock
//    bool result1 = __controller->IsControllerInitialized();
//    bool result2 = __controller->IsSemiAutoActivated();
//    foreach (QWidget* var, __busyInterlock) {
//        var->setEnabled(__controller->IsSemiAutoActivated() &&
//                        __controller->IsControllerInitialized() &&
//                        ui->tableViewUnit->selectionModel()->hasSelection());
//    }
//    //! Once on working , cannot override to off
//    ui->pushButtonWorking->setEnabled(ui->pushButtonWorking->isEnabled() &&
//                                      !umb.Value(__statusShowMap[ui->pushButtonWorking].first.getAddress()).toBool());
//}

void FrontUnitPanel::onCommandClick()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());

    UnitOperationBlock block(&m_monitor);
    //!Fetch last status
//    *reinterpret_cast<CellDataBlock*>(block.Anchor()) = m_monitor;
//    *static_cast<CellDataBlock*>(&uob) = __controller->MonitorBlock();//update latest value
    //! Flip
    block.Value(m_controlMap[button],
            QVariant::fromValue(!block.Value(m_controlMap[button]).toBool()));
    //! Set mode
    m_controller->setProperty(QVariant::fromValue(ManualModeDataBlock::COMMIT_MODE).toString().toStdString().c_str(),
                                  QVariant::fromValue(ManualModeDataBlock::MODE_EXE_UNIT));
    m_controller->setProperty(QVariant::fromValue(ManualModeDataBlock::COMMIT_CATEGRORY).toString().toStdString().c_str(),
                                  QVariant::fromValue(ManualModeDataBlock::SELECTION_UNIT));
    m_controller->setProperty(QVariant::fromValue(ManualModeDataBlock::COMMIT_DEVICE_INDEX).toString().toStdString().c_str(),
                                  currentIndex());
    m_controller->setProperty(QVariant::fromValue(ManualModeDataBlock::DATA_BLOCK_HEAD).toString().toStdString().c_str(),
                                  QVariant::fromValue(*reinterpret_cast<CellDataBlock*>(block.Anchor())));
    m_controller->setProperty(QVariant::fromValue(ManualModeDataBlock::BIT_1_RUN).toString().toStdString().c_str(),
                                  true);
}

void FrontUnitPanel::showEvent(QShowEvent *event)
{
    //! Semi-auto mode(Keep unit running
    m_controller->setProperty(QVariant::fromValue(ManualModeDataBlock::BIT_2_ENGAGED_MANUAL).toString().toStdString().c_str(),
                              false);

    FrontCommonManual::showEvent(event);
}

int FrontUnitPanel::currentIndex()
{
    //!Renew current index
    if(ui->tableViewUnit->selectionModel()->hasSelection())
    {
        auto table = static_cast<QSqlTableModel*>(ui->tableViewUnit->model());
        auto record = table->record(ui->tableViewUnit->selectionModel()->selectedRows().first().row());
        m_index = record.value(QVariant::fromValue(HEADER_STRUCTURE::ID).toString()).toInt();
    }
    return m_index;
}

