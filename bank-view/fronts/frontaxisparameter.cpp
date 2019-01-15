#include "frontaxisparameter.h"
#include "ui_frontaxisparameter.h"

FrontAxisParameter::FrontAxisParameter(QWidget *parent) :
    FrontCommonManual(parent),
    ui(new Ui::FrontAxisParameter)
{
    ui->setupUi(this);

	m_auth->addDisableWidget(AUTH::ROLE_OPERATOR, this);

    //! Manual mode required
	onPropertyChanged(QVariant::fromValue(HEADER_STRUCTURE::STATE_MANUAL),true);
    //!
    m_categrory = ManualModeDataBlock::SELECTION_AXIS;
    m_monitorOperation = utilities::listupEnumVariant<AxisMonitorBlock::MONITOR_OPERATION>();
	m_monitorOperation << utilities::listupEnumVariant<AxisMonitorBlock::RUN_STATUS>();
    //! QPushButton link
	m_bankButtons = findChildren<QPushButton*>(QRegExp("\\w+Bank\\w+"));
    foreach (QPushButton* var, m_bankButtons) {
        //!
        //! \brief connect
        //! no jog mode/stop
        connect(var,&QPushButton::clicked,this,&FrontAxisParameter::onBankExecution);
    }
    foreach (QPushButton* var, findChildren<QPushButton*>(QRegExp("\\w+Direct\\w+"))) {
        connect(var,&QPushButton::clicked,this,&FrontAxisParameter::onDirectExecution);
    }

    connect(ui->pushButtonDirectForward,&QPushButton::clicked,this,&FrontAxisParameter::onInterrupted);
    connect(ui->pushButtonDirectBackward,&QPushButton::clicked,this,&FrontAxisParameter::onInterrupted);
    connect(ui->pushButtonStop,&QPushButton::clicked,this,&FrontAxisParameter::onInterrupted);

    connect(ui->pushButtonAxisSet,&QPushButton::clicked,this,&FrontAxisParameter::onAxisSet);
    //! Map
    m_lcdMap[AxisMonitorBlock::OFFSET_MONITOR_POS_COMMAND] = ui->lcdNumberPositionReference;
    m_lcdMap[AxisMonitorBlock::OFFSET_MONITOR_POS_FEEDBACK] = ui->lcdNumberPositionFeedback;
    m_lcdMap[AxisMonitorBlock::OFFSET_MONITOR_SPD_FEEDBACK] = ui->lcdNumberSpeedFeedback;
    m_lcdMap[AxisMonitorBlock::OFFSET_MONITOR_TOR_FEEDBACK] = ui->lcdNumberTorqueFeedback;
    //!
    connect(ui->tabWidgetCommandPanel,&QTabWidget::currentChanged,this,&FrontAxisParameter::onTabCurrentChanged);
    //! Going to polishs
    foreach (QWidget* var, findChildren<QPushButton*>()) {
        m_widgetsPolish.append(var);
    }
	m_widgetsPolish.append(ui->textBrowserAlarmWarning);
	//! Widget lock list
	for each (QPushButton* var in findChildren<QPushButton*>())
	{
		m_widgetLockList.append(var);
	}
	for each (QDoubleSpinBox* var in findChildren<QDoubleSpinBox*>())
	{
		m_widgetLockList.append(var);
	}
	m_widgetLockList.removeOne(ui->pushButtonStop);
}
//!
//! \brief FrontAxisParameter::Setup
//! \param commandBlockTable
//! \param axisTable
//! \param regionTable
//! \param axisErrorTable
//!
void FrontAxisParameter::Setup(QSqlTableModel *commandBlockTable,
                               QSqlTableModel *axisTable,
                               QSqlTableModel *regionTable,
                               QSqlTableModel *axisErrorTable,
                               QSqlTableModel *commandBlockTableHeader,
                               QSqlTableModel *axisTableHeader)
{
    mainDataTable = commandBlockTable;
    m_commblockAdaptor = new GenericSqlTableAdapter<ExtendedCommandBlock,CommandBlock::DataBaseHeaders>(mainDataTable);

    ui->tableViewCommandBlock->setModel(mainDataTable);
    HEADER_STRUCTURE::HeaderRender::renderViewHeader(commandBlockTableHeader,ui->tableViewCommandBlock);
    //! Would not influce the setting axis table
    QSqlTableModel* cloneAxisTable = new QSqlTableModel(ui->widgetFilter,
                                                        axisTable->database());
    cloneAxisTable->setTable(axisTable->tableName());

    ui->widgetFilter->Setup(mainDataTable,
                            QVariant::fromValue(CommandBlock::AXIS_ID),
                            cloneAxisTable,
                            QVariant::fromValue(AxisBlock::REGION),
                            regionTable);
    //!
    connect(ui->widgetFilter,&FrontTwinFilter2::key1Selected,this,&FrontAxisParameter::onMonitorIndexChanged);
    //!
    m_axisTable = axisTable;
    m_axisErrorTable = axisErrorTable;
    //!
    ui->tableViewAxis->setModel(axisTable);
    HEADER_STRUCTURE::HeaderRender::renderViewHeader(axisTableHeader,
                                                     ui->tableViewAxis);
}

FrontAxisParameter::~FrontAxisParameter()
{
    delete ui;
}
//!
//! \brief FrontAxisParameter::onBankExecution
//! Setup record
void FrontAxisParameter::onBankExecution()
{
    QSqlRecord record = mainDataTable->record(selectedCommandBlockIndex());

    if(sender()==ui->pushButtonBankCoordinateSet)
    {
        //setup field of coordinate
        record.setValue(QVariant::fromValue(CommandBlock::COORD1).toString(),ui->lcdNumberPositionFeedback->value());
    }
    else if(sender()==ui->pushButtonBankParameterSet)
    {
        //setup field of parameters
        record.setValue(QVariant::fromValue(CommandBlock::SPEED).toString(),ui->doubleSpinBoxSpeedReference->value());
        record.setValue(QVariant::fromValue(CommandBlock::ACC_TIME).toString(),ui->doubleSpinBoxAccerlation->value());
        record.setValue(QVariant::fromValue(CommandBlock::DEC_TIME).toString(),ui->doubleSpinBoxDecerlation->value());
        record.setValue(QVariant::fromValue(CommandBlock::TORQUE_LIMIT).toString(),ui->doubleSpinBoxTorque->value());
    }

    //! cannot put while loop here , once setRecord fails , should call select to refresh data
    if(!mainDataTable->setRecord(selectedCommandBlockIndex(),record))
    {
        qDebug() << QString("Bank set error:%1").arg(mainDataTable->lastError().text());
    }
    mainDataTable->select(); //re-load model
}
//!
//! \brief FrontAxisParameter::onDirectExecution
//! Run for controller
//! pushButtonDirectExecution
//! pushButtonDirectAbsolute
//! pushButtonDirectStepMinus
//! pushButtonDirectStepPlus
//! pushButtonDirectAlarmClear
//! pushButtonDirectServoOn
void FrontAxisParameter::onDirectExecution(bool value)
{   
    CellDataBlock data;
    AxisOperationBlock m_operationBlock(reinterpret_cast<MODBUS_U_WORD*>(&data));
    PosCommandBlock m_postionBlock(reinterpret_cast<MODBUS_U_WORD*>(&data));

    //! Categrory
    ManualModeDataBlock::Categrories categrory = ManualModeDataBlock::SELECTION_AXIS;
    //! Mode
    ManualModeDataBlock::Mode mode = ManualModeDataBlock::MODE_EXE_AXIS;
    //! Prepare Commit Mode/Index , DataBlock , by different push button

    //! Axis index
	emit m_port->externalPropertyChange(QVariant::fromValue(ManualModeDataBlock::COMMIT_DEVICE_INDEX),
		currentIndex());

    //! Data block
    if(sender()==ui->pushButtonDirectAlarmClear )
    {
        m_operationBlock.Value(AxisOperationBlock::OP_BIT15_ALARM_CLEAR,true);
    }
    else if(sender()==ui->pushButtonDirectServoOn)
    {
        m_operationBlock.Value(AxisOperationBlock::OP_BIT0_SERVO_ON,true);
    }
    else
    {
		//! Command block series

        categrory = ManualModeDataBlock::SELECTION_COMMAND_BLOCK;
        mode = ManualModeDataBlock::MODE_EXE_COMMAND_BLOCK;
        //! Prepare parameters
        setCommonParameters(m_postionBlock);

        if(sender()==ui->pushButtonDirectExecution)
        {
            int m_commandBlockId = mainDataTable->record(selectedCommandBlockIndex())
                    .value(utilities::trimNamespace(QVariant::fromValue(CommandBlock::ID)))
                    .toInt();

			*reinterpret_cast<CellDataBlock*>(m_postionBlock.Anchor()) = 
				*reinterpret_cast<CellDataBlock*>( m_commblockAdaptor->Record(m_commandBlockId,
                                       AbstractSqlTableAdpater::KEY_NAMED_KEY,
                                       QVariant::fromValue(CommandBlock::ID)).Anchor());

			//! Resume current filter
			m_commblockAdaptor->Model()->setFilter(currentFilter());
			m_commblockAdaptor->Model()->select();
        }
        else if(sender()==ui->pushButtonDirectAbsolute)
        {
            m_postionBlock.Value(PosCommandBlock::OFFSET_POS_ABS_REL,true);
            m_postionBlock.Value(PosCommandBlock::OFFSET_ECB_COORD1,ui->doubleSpinBoxCoordinate->value());
        }
        else if((sender()==ui->pushButtonDirectForward ||
                sender()==ui->pushButtonDirectBackward))
        {
            //! Should be checked
            if(!value)
                return;// not execution

            //Positive limit/Negtive limit
            m_postionBlock.Value(PosCommandBlock::OFFSET_ECB_COORD1,
                                 ((sender()==ui->pushButtonDirectForward) ?
                                     selectedAxisValue(ui->widgetFilter->SelectedKey1(),QVariant::fromValue(AxisBlock::LIMIT_PLUS)) :
                                     selectedAxisValue(ui->widgetFilter->SelectedKey1(),QVariant::fromValue(AxisBlock::LIMIT_MINUS))));
            m_postionBlock.Value(PosCommandBlock::OFFSET_POS_ABS_REL,true);//Always in ABS
        }
		else if (sender() == ui->pushButtonDirectZero)
		{
			SetZeroCommandBlock m_zeroBlock(reinterpret_cast<MODBUS_U_WORD*>(&data));
		}
        else
        {
            //! Step mode
            m_postionBlock.Value(PosCommandBlock::OFFSET_POS_ABS_REL,false);
            m_postionBlock.Value(PosCommandBlock::OFFSET_ECB_COORD1,
                                 (ui->doubleSpinBoxStep->value()*
                                 ((sender()==ui->pushButtonDirectStepPlus) ? 1 : -1)));
        }
    }

    //! Set mode
    /*m_port->setProperty(QVariant::fromValue(ManualModeDataBlock::COMMIT_MODE).toString().toStdString().c_str(),
                              QVariant::fromValue(mode));*/
	emit m_port->externalPropertyChange(QVariant::fromValue(ManualModeDataBlock::COMMIT_MODE),
		QVariant::fromValue(mode));

    //! Set categrory
    /*m_port->setProperty(QVariant::fromValue(ManualModeDataBlock::COMMIT_CATEGRORY).toString().toStdString().c_str(),
                              QVariant::fromValue(categrory));*/
	emit m_port->externalPropertyChange(QVariant::fromValue(ManualModeDataBlock::COMMIT_CATEGRORY),
		QVariant::fromValue(categrory));

    //! Fire
    /*m_port->setProperty(QVariant::fromValue(ManualModeDataBlock::DATA_BLOCK_HEAD).toString().toStdString().c_str(),
                              QVariant::fromValue(data));*/
	emit m_port->externalPropertyChange(QVariant::fromValue(ManualModeDataBlock::DATA_BLOCK_HEAD),
		QVariant::fromValue(data));


    /*m_controller->setProperty(QVariant::fromValue(ManualModeDataBlock::BIT_1_RUN).toString().toStdString().c_str(),
                              true);*/
	emit qobject_cast<PropertyPortCommon*>(m_port)->externalPropertyChange(QVariant::fromValue(ManualModeDataBlock::BIT_1_RUN),
		true);
}

void FrontAxisParameter::onAxisSet()
{
    if(!ui->tableViewAxis->selectionModel()->hasSelection())
        return;
    QSqlRecord record =
            m_axisTable->record(0);
    qDebug() << record.value("ID");

    record.setValue(ui->tableViewAxis->selectionModel()->selectedColumns().first().column(),
                    ui->lcdNumberPositionFeedback->value());
    if(!m_axisTable->setRecord(0,record))
        qDebug() << QString("onAxisSet , error , %1").arg(m_axisTable->lastError().text());
    m_axisTable->select();
}

void FrontAxisParameter::onInterrupted(bool value)
{
    if(value)
        return; //execute when unchecked

    /*m_port->setProperty(QVariant::fromValue(ManualModeDataBlock::BIT_0_ENGAGED_HMI).toString().toStdString().c_str(),
                              false);*/
	emit m_port->externalPropertyChange(QVariant::fromValue(ManualModeDataBlock::BIT_0_ENGAGED_HMI),
		false);

}


void FrontAxisParameter::setCommonParameters(PosCommandBlock& block)
{
    block.Value(AbstractCommandBlock::OFFSET_ACB_AXIS_ID,currentIndex());
    block.Value(AbstractCommandBlock::OFFSET_ACB_SPD,ui->doubleSpinBoxSpeedReference->value());//unit/sec
    block.Value(AbstractCommandBlock::OFFSET_ACB_ACC_T,ui->doubleSpinBoxAccerlation->value());//ms
    block.Value(AbstractCommandBlock::OFFSET_ACB_DEC_T,ui->doubleSpinBoxDecerlation->value());//ms
    block.Value(AbstractCommandBlock::OFFSET_ACB_TOR_LIMIT,ui->doubleSpinBoxTorque->value());// 0.01%
}

int FrontAxisParameter::selectedCommandBlockIndex() const
{
    return ui->tableViewCommandBlock->selectionModel()->selectedRows().first().row();
}
QVariant FrontAxisParameter::selectedAxisValue(int axisId,QVariant key) const
{
    return utilities::getSqlTableSelectedRecord(m_axisTable,
                                                QVariant::fromValue(AxisBlock::ID),
                                                axisId)
            .value(utilities::trimNamespace(key));
}

//!
//! \brief FrontAxisParameter::dynamicPropertyChanged
//! \param key
//! \param value
//!
void FrontAxisParameter::dynamicPropertyChanged(int key,QVariant value)
{
    switch (key) {
    case ManualModeDataBlock::MONITOR_BLOCK_HEAD:
    {
        *(static_cast<AbstractDataBlock*>(&m_monitorBlock)) =
                value.value<CellDataBlock>(); //value assignment

        foreach (AxisMonitorBlock::OffsetMonitor var, m_lcdMap.keys()) {
            m_lcdMap[var]->display(m_monitorBlock.Value(var).toReal());
        }

        //! show axis error
        //! Alarm/Warning details
        QString message =
                utilities::getSqlTableSelectedRecord(m_axisErrorTable,QVariant::fromValue(HEADER_STRUCTURE::ID),m_monitorBlock.Value(AxisMonitorBlock::OFFSET_MONITOR_WARNINGS)).value(QVariant::fromValue(HEADER_STRUCTURE::zh_TW).toString()).toString();
        ui->textBrowserAlarmWarning->setText(QString("%1").arg(message));

        //! Self-raise Bit properties
        foreach (QVariant var, m_monitorOperation) {
			onPropertyChanged(var, m_monitorBlock.Value(var.toUInt()).toBool());
        }
        break;
    }
    default:
        break;
    }
}

int FrontAxisParameter::currentIndex()
{
    return ui->widgetFilter->SelectedKey1();
}

QString FrontAxisParameter::currentFilter() const
{
    return ui->widgetFilter->Filter1();
}

void FrontAxisParameter::onMonitorIndexChanged()
{
    m_axisTable->setFilter(utilities::generateFilterString(QVariant::fromValue(HEADER_STRUCTURE::ID),
                                                           currentIndex()));
    m_axisTable->select();

    FrontCommonManual::onMonitorIndexChanged();
}
//!
//! \brief FrontAxisParameter::onTabCurrentChanged
//! Transmit axis parameter
void FrontAxisParameter::onTabCurrentChanged()
{
    if(ui->tabWidgetCommandPanel->currentWidget() !=
            ui->tabAxisSetting){
		//!trigger axis parameter transfer
        /*m_port->setProperty(QVariant::fromValue(ManualModeDataBlock::BATCH_PRESCHEDUALED_MODE).toString().toStdString().c_str(),
                                  true);*/
		emit m_port->externalPropertyChange(QVariant::fromValue(ManualModeDataBlock::BATCH_PRESCHEDUALED_MODE),
			true);
    }
    else
    {
        //!Switch to current tab
        onMonitorIndexChanged();
    }
}
//!
void FrontAxisParameter::onTimerScan()
{
	FrontCommonManual::onTimerScan(); //base method

	bool isSelected = ui->tableViewCommandBlock->selectionModel()->hasSelection();
	foreach (QPushButton* var , m_bankButtons)
	{	
		var->setEnabled(isSelected && this->isEnabled());
	}
	ui->pushButtonDirectExecution->setEnabled(isSelected && this->isEnabled());
	//! Axis error message
	MODBUS_U_LONG errorCode = m_monitorBlock.Value(AxisMonitorBlock::OFFSET_MONITOR_WARNINGS).value<MODBUS_U_LONG>();
	QString queriedString = utilities::getSqlTableSelectedRecord(m_axisErrorTable,
		QVariant::fromValue(HEADER_STRUCTURE::ID),
		errorCode).value(QVariant::fromValue(HEADER_STRUCTURE::zh_TW).toString()).toString();

	if (queriedString.isEmpty() || queriedString.isNull())
		queriedString = QString("0x%1").arg(QString::number(errorCode, 16)); //represented in hex
	ui->textBrowserAlarmWarning->setText(queriedString);
}