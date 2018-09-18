#include "frontaxisparameter.h"
#include "ui_frontaxisparameter.h"

FrontAxisParameter::FrontAxisParameter(QWidget *parent) :
    FrontCommonManual(parent),
    ui(new Ui::FrontAxisParameter)
{
    ui->setupUi(this);

    //! QPushButton link
    foreach (QPushButton* var, findChildren<QPushButton*>(QRegExp("\\w+Bank\\w+"))) {
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

    //! Map
    m_lcdMap[AxisMonitorBlock::OFFSET_MONITOR_POS_COMMAND] = ui->lcdNumberPositionReference;
    m_lcdMap[AxisMonitorBlock::OFFSET_MONITOR_POS_FEEDBACK] = ui->lcdNumberPositionFeedback;
    m_lcdMap[AxisMonitorBlock::OFFSET_MONITOR_SPD_FEEDBACK] = ui->lcdNumberSpeedFeedback;
    m_lcdMap[AxisMonitorBlock::OFFSET_MONITOR_TOR_FEEDBACK] = ui->lcdNumberTorqueFeedback;
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
                               QSqlTableModel *commandBlockTableHeader)
{
    mainDataTable = commandBlockTable;
    m_commblockAdaptor = new GenericSqlTableAdapter<ExtendedCommandBlock,CommandBlock::DataBaseHeaders>(mainDataTable);

    ui->tableViewCommandBlock->setModel(mainDataTable);
    HEADER_STRUCTURE::HeaderRender::renderViewHeader(commandBlockTableHeader,ui->tableViewCommandBlock);
    //!
    ui->widgetFilter->Setup(mainDataTable,
                            QVariant::fromValue(CommandBlock::AXIS_ID),
                            axisTable,
                            QVariant::fromValue(AxisBlock::REGION),
                            regionTable);
    //!
    connect(ui->widgetFilter,&FrontTwinFilter2::key1Selected,this,&FrontAxisParameter::onMonitorIndexChanged);
    //!
    m_axisTable = axisTable;
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
    AxisOperationBlock m_operationBlock;
    PosCommandBlock m_postionBlock;

    AbstractDataBlock* data = &(m_operationBlock);

    //!
    m_controller->setProperty(QVariant::fromValue(ManualModeDataBlock::COMMIT_CATEGRORY).toString().toStdString().c_str(),
                              QVariant::fromValue(m_categrory));

    ManualModeDataBlock::Mode mode;
    //! Prepare Commit Mode/Index , DataBlock , by different push button
    mode = (sender()==ui->pushButtonDirectAlarmClear ||
            sender()==ui->pushButtonDirectServoOn ) ?
                ManualModeDataBlock::MODE_EXE_AXIS :
                ManualModeDataBlock::MODE_EXE_COMMAND_BLOCK;
    //! Set mode
    m_controller->setProperty(QVariant::fromValue(ManualModeDataBlock::COMMIT_MODE).toString().toStdString().c_str(),
                              QVariant::fromValue(mode));
    //! Data block
    if(sender()==ui->pushButtonDirectAlarmClear )
    {
        m_operationBlock.Value(AxisOperationBlock::OP_ALARM_CLEAR,true);
    }
    else if(sender()==ui->pushButtonDirectServoOn)
    {
        m_operationBlock.Value(AxisOperationBlock::OP_SERVO_ON,true);
    }
    else
    {
        //! Link
        data = &(m_postionBlock);
        //! Prepare parameters
        setCommonParameters(m_postionBlock);

        if(sender()==ui->pushButtonDirectExecution)
        {
            int m_commandBlockId = mainDataTable->record(selectedCommandBlockIndex())
                    .value(utilities::trimNamespace(QVariant::fromValue(CommandBlock::ID)))
                    .toInt();
            m_commblockAdaptor->Record(m_commandBlockId,
                                       AbstractSqlTableAdpater::KEY_NAMED_KEY,
                                       QVariant::fromValue(CommandBlock::ID));
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
        else
        {
            //! Step mode
            m_postionBlock.Value(PosCommandBlock::OFFSET_POS_ABS_REL,false);
            m_postionBlock.Value(PosCommandBlock::OFFSET_ECB_COORD1,
                                 (ui->doubleSpinBoxStep->value()*
                                 ((sender()==ui->pushButtonDirectStepPlus) ? 1 : -1)));
        }
    }

    //! Fire
    m_controller->setProperty(QVariant::fromValue(ManualModeDataBlock::DATA_BLOCK_HEAD).toString().toStdString().c_str(),
                              QVariant::fromValue(*reinterpret_cast<CellDataBlock*>(data->Anchor())));
    m_controller->setProperty(QVariant::fromValue(ManualModeDataBlock::BIT_1_RUN).toString().toStdString().c_str(),
                              true);
}

void FrontAxisParameter::onInterrupted(bool value)
{
    if(value)
        return; //execute when unchecked

    m_controller->setProperty(QVariant::fromValue(ManualModeDataBlock::BIT_0_ENGAGED_HMI).toString().toStdString().c_str(),
                              false);
}


void FrontAxisParameter::setCommonParameters(PosCommandBlock& block)
{
    block.Value(AbstractCommandBlock::OFFSET_ACB_AXIS_ID,ui->widgetFilter->SelectedKey1());
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
                                                ui->widgetFilter->SelectedKey1())
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
        *(static_cast<AbstractDataBlock*>(&m_monitorBlock)) =
                value.value<CellDataBlock>(); //value assignment

        foreach (AxisMonitorBlock::OffsetMonitor var, m_lcdMap.keys()) {
            m_lcdMap[var]->display(m_monitorBlock.Value(var).toReal());
        }

        //! Todo , show axis error

        break;
    default:
        break;
    }
}

int FrontAxisParameter::currentIndex() const
{
    return ui->widgetFilter->SelectedKey1();
}

QString FrontAxisParameter::currentFilter() const
{
    return ui->widgetFilter->Filter1();
}
