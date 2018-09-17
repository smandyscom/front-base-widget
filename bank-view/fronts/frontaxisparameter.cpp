#include "frontaxisparameter.h"
#include "ui_frontaxisparameter.h"

FrontAxisParameter::FrontAxisParameter(QWidget *parent) :
    FrontCommon(parent),
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
void FrontAxisParameter::onDirectExecution()
{
    //!
    m_controller->setProperty(QVariant::fromValue(ManualModeDataBlock::COMMIT_CATEGRORY).toString().toStdString().c_str(),
                              QVariant::fromValue(m_categrory));

    ManualModeDataBlock mode;
    //! Prepare Commit Mode/Index , DataBlock , by different push button
    if(sender()==ui->pushButtonDirectAlarmClear ||
            sender()==ui->pushButtonDirectServoOn )
    {
        mode = ManualModeDataBlock::MODE_EXE_AXIS;
    }
    else
    {
        mode = ManualModeDataBlock::MODE_EXE_COMMAND_BLOCK;
    }
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
    else if(sender()==ui->pushButtonDirectExecution)
    {

    }
    else
    {
        //! Prepare parameters
        PosCommandBlock m_postionBlock;
        setCommonParameters(block);

        if(button==ui->pushButtonDirectAbsolute)


    }

    //! Fire
    m_controller->setProperty(QVariant::fromValue(ManualModeDataBlock::DATA_BLOCK_HEAD).toString().toStdString().c_str(),QVariant::fromValue(*m_block));
    m_controller->setProperty(QVariant::fromValue(ManualModeDataBlock::BIT_1_RUN).toString().toStdString().c_str(),true);
}

void FrontAxisParameter::setCommonParameters(PosCommandBlock& block)
{
    block.Value(AbstractCommandBlock::OFFSET_ACB_AXIS_ID,ui->widgetFilter->SelectedPrimaryId());
    block.Value(AbstractCommandBlock::OFFSET_ACB_SPD,ui->doubleSpinBoxSpeedReference->value());//unit/sec
    block.Value(AbstractCommandBlock::OFFSET_ACB_ACC_T,ui->doubleSpinBoxAccerlation->value());//ms
    block.Value(AbstractCommandBlock::OFFSET_ACB_DEC_T,ui->doubleSpinBoxDecerlation->value());//ms
    block.Value(AbstractCommandBlock::OFFSET_ACB_TOR_LIMIT,ui->doubleSpinBoxTorque->value());// 0.01%
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
                value.value<CellDataBlock>();

        foreach (AxisMonitorBlock::OffsetMonitor var, m_lcdMap.keys()) {
            m_lcdMap[var]->display(m_monitorBlock.Value(var).toReal());
        }

        //! Todo , show axis error

        break;
    default:
        break;
    }
}
