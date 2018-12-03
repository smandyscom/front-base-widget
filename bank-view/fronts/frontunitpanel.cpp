#include "frontunitpanel.h"
#include "ui_frontunitpanel.h"

FrontUnitPanel::FrontUnitPanel(QWidget *parent) :
    FrontCommonManual(parent),
    ui(new Ui::FrontUnitPanel),
    m_index(0)
{
    ui->setupUi(this);
    //!
    m_categrory = ManualModeDataBlock::SELECTION_UNIT;
    m_condition =  utilities::listupEnumVariant<UnitMonitorBlock::TransitionBits>();
    m_status =  utilities::listupEnumVariant<UnitOperationBlock::ControlBits>();
    //!
    m_controlMap[ui->pushButtonEnableStep] = (UnitOperationBlock::OFFSET_UOB_EN_STEP);
    m_controlMap[ui->pushButtonStep] = (UnitOperationBlock::OFFSET_UOB_TRIG_STEP);
    m_controlMap[ui->pushButtonWorking] = (UnitOperationBlock::OFFSET_UOB_WORKING_OVERRIDE);
    m_controlMap[ui->pushButtonPause] = (UnitOperationBlock::OFFSET_UOB_STATE_PAUSE);
    m_controlMap[ui->pushButtonMaterial] = (UnitOperationBlock::OFFSET_UOB_MATERIAL_OVERRIDE);
    m_controlMap[ui->pushButtonByPass] = (UnitOperationBlock::OFFSET_UOB_IS_BYPASSED);

    foreach (QPushButton* var, m_controlMap.keys()) {
        connect(var,SIGNAL(clicked(bool)),this,SLOT(onCommandClick()));
        m_widgetsPolish.append(var);
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
                           QSqlTableModel* unitTableHeader,
							QSqlTableModel* unitConfig,
							QSqlTableModel* unitConfigHeader)
{
    mainDataTable = unitTable;
    //
    ui->tableViewUnit->setModel(unitTable);
    connect(ui->tableViewUnit,&QTableView::clicked,this,&FrontUnitPanel::onMonitorIndexChanged);
    //!
    HEADER_STRUCTURE::HeaderRender::renderViewHeader(unitTableHeader,
                                                     ui->tableViewUnit);
	//!
	ui->tableViewConfig->setModel(unitConfig);
	HEADER_STRUCTURE::HeaderRender::renderViewHeader(unitConfigHeader,
		ui->tableViewConfig);
}

//void FrontUnitPanel::onTimerTimeout()
//{


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

    UnitOperationBlock block(m_monitorBlock.Anchor());
    bool value = block.Value(m_controlMap[button]).toBool();
    //! Flip
    block.Value(m_controlMap[button],!value);
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

int FrontUnitPanel::currentIndex()
{
    //!Renew current index
    if(ui->tableViewUnit->selectionModel()->hasSelection())
    {
        auto table = static_cast<QSqlTableModel*>(ui->tableViewUnit->model());
        auto record = table->record(ui->tableViewUnit->selectionModel()->selectedIndexes().first().row());
        m_index = record.value(QVariant::fromValue(HEADER_STRUCTURE::ID).toString()).toInt();

		//! switch configuration
		QSqlTableModel* m_configTable = qobject_cast<QSqlTableModel*>(ui->tableViewConfig->model());
		m_configTable->setFilter(utilities::generateFilterString(QVariant::fromValue(HEADER_STRUCTURE::ID), m_index));
		m_configTable->select();

    }
    return m_index;
}

void FrontUnitPanel::dynamicPropertyChanged(int key, QVariant value)
{
    switch (key) {
    case ManualModeDataBlock::MONITOR_BLOCK_HEAD:
    {
        *(static_cast<AbstractDataBlock*>(&m_monitorBlock)) =
                value.value<CellDataBlock>(); //value assignment

        //! Self-raise Bit properties
        foreach (QVariant var, m_status) {
            setProperty(var.toString().toStdString().c_str(),
                        m_monitorBlock.Value(var.toUInt()).toBool());
        }
        foreach (QVariant var, m_condition) {
            setProperty(var.toString().toStdString().c_str(),
                        m_monitorBlock.Value(var.toUInt()).toBool());
        }
        //!
        ui->lcdNumberState->display(m_monitorBlock.Value(UnitMonitorBlock::OFFSET_MONITOR_STATE).toInt());
        ui->lcdNumberNextState->display(m_monitorBlock.Value(UnitMonitorBlock::OFFSET_MONITOR_NEXT_STATE).toInt());
        ui->lcdNumberCycleTime->display(m_monitorBlock.Value(UnitMonitorBlock::OFFSET_MONITOR_WORKING_TIMER_CACHE).toReal());
        break;
    }
    default:
        break;
    }
	//!Base method
	FrontCommonManual::dynamicPropertyChanged(key, value);
}
