#include "frontunitpanel.h"
#include "ui_frontunitpanel.h"

FrontUnitPanel::FrontUnitPanel(QWidget *parent) :
    FrontCommonManual(parent),
    ui(new Ui::FrontUnitPanel),
    m_index(0)
{
    ui->setupUi(this);

	m_auth->addDisableWidget(AUTH::ROLE_OPERATOR, this);
    //!
    m_categrory = ManualModeDataBlock::SELECTION_UNIT;
    m_condition =  utilities::listupEnumVariant<UnitMonitorBlock::TransitionBits>();
	m_condition <<  utilities::listupEnumVariant<UnitOperationBlock::ControlBits>();
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
	QRegExp rx("labelCondition[0-7]");
	m_widgetsPolish.append(findChildren<QWidget*>(rx));
	//!
	for each (QPushButton* var in findChildren<QPushButton*>())
	{
		m_widgetLockList.append(var);
	}
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

void FrontUnitPanel::onTimerScan()
{
    //! Once on working , cannot override to off
    ui->pushButtonWorking->setEnabled(ui->pushButtonWorking->isEnabled() &&
                                      !property(QVariant::fromValue(UnitOperationBlock::OFFSET_UOB_WORKING_OVERRIDE).toString().toStdString().c_str()).toBool());
	FrontCommonManual::onTimerScan();
}

void FrontUnitPanel::onCommandClick()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());

    UnitOperationBlock block(m_monitorBlock.Anchor());
    bool value = block.Value(m_controlMap[button]).toBool();
    //! Flip
    block.Value(m_controlMap[button],!value);
    //! Set mode
	emit m_port->externalPropertyChange(QVariant::fromValue(ManualModeDataBlock::COMMIT_MODE),
		QVariant::fromValue(ManualModeDataBlock::MODE_EXE_UNIT));
	emit m_port->externalPropertyChange(QVariant::fromValue(ManualModeDataBlock::COMMIT_CATEGRORY),
		QVariant::fromValue(ManualModeDataBlock::SELECTION_UNIT));
	emit m_port->externalPropertyChange(QVariant::fromValue(ManualModeDataBlock::COMMIT_DEVICE_INDEX),
		currentIndex());
	emit m_port->externalPropertyChange(QVariant::fromValue(ManualModeDataBlock::DATA_BLOCK_HEAD),
		QVariant::fromValue(*reinterpret_cast<CellDataBlock*>(block.Anchor())));
	emit m_port->externalPropertyChange(QVariant::fromValue(ManualModeDataBlock::BIT_1_RUN),
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
		m_configTable->setFilter(utilities::generateFilterString(QVariant::fromValue(UnitConfigBlock::UNIT_ID), m_index));

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
}
