#include "frontmanaualmode.h"
#include "ui_frontmanaualmode.h"
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QPalette>
#include <utilities.h>
#include <QApplication>
FrontManaualMode::FrontManaualMode(QSqlRelationalTableModel *wholeCommandBankModel,
                                   QSqlRelationalTableModel *wholeAxisBankModel,
                                   QSqlRelationalTableModel *regionModel,
                                   QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrontManaualMode),
    __commandBlockTableFront(wholeCommandBankModel),
    __axisTableFront(wholeAxisBankModel),
    __regionTable(regionModel)
{
    ui->setupUi(this);
    //! Link
    __controller = ControllerManualMode::Instance();
    __bankTransfer= ControllerBankTransfer::Instance();
    //! Resued widgets
    new FrontBankTransfer(CommitBlock::SELECTION_COMMAND_BLOCK,ui->widgetBankTransfer);
    FrontTwinFilter* __ftf = new FrontTwinFilter(wholeCommandBankModel,
                                                QVariant::fromValue(CommandBlock::AXIS_ID),
                                                wholeAxisBankModel,
                                                QVariant::fromValue(REGION),
                                                regionModel,
                                                ui->widgetFilter);
    connect(__ftf,&FrontTwinFilter::primarySelected,[=](QVariant key){
        __selectedAxisId = key.value<MODBUS_WORD>();
        __controller->onMonitorDeviceIndexChanged(__selectedAxisId);
    });
    //!setup
    __commitOption.Mode(CommitBlock::MODE_EXE_COMMAND_BLOCK);

    //!
    //! \brief connect
    //!
    connect(ui->pushButtonAbsolute,SIGNAL(clicked(bool)),this,SLOT(onManualOperationClicked()));
    connect(ui->pushButtonStepMinus,SIGNAL(clicked(bool)),this,SLOT(onManualOperationClicked()));
    connect(ui->pushButtonStepPlus,SIGNAL(clicked(bool)),this,SLOT(onManualOperationClicked()));
    connect(ui->pushButtonZret,SIGNAL(clicked(bool)),this,SLOT(onManualOperationClicked()));
    connect(ui->pushButtonStop,SIGNAL(clicked(bool)),__controller,SLOT(onInterrupted()));

    connect(ui->pushButtonFeedForward,SIGNAL(pressed()),this,SLOT(onManualOperationClicked()));
    connect(ui->pushButtonFeedBackward,SIGNAL(pressed()),this,SLOT(onManualOperationClicked()));
    connect(ui->pushButtonFeedForward,SIGNAL(released()),__controller,SLOT(onInterrupted()));
    connect(ui->pushButtonFeedBackward,SIGNAL(released()),__controller,SLOT(onInterrupted()));

    connect(ui->pushButtonCoordinateSet,SIGNAL(clicked(bool)),this,SLOT(onBankOperationClicked()));
    connect(ui->pushButtonParameterSet,SIGNAL(clicked(bool)),this,SLOT(onBankOperationClicked()));
    connect(ui->pushButtonBankExecution,SIGNAL(clicked(bool)),this,SLOT(onBankOperationClicked()));
    //!
    __timer = new QTimer(this);
    connect(__timer,SIGNAL(timeout()),this,SLOT(onTimerTimeout()));
    __timer->start(100);//every 100 ms update once

    ui->tableViewCommandBlock->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableViewCommandBlock->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableViewCommandBlock->setModel(__commandBlockTableFront);
    QList<CommandBlock::DataBaseHeaders> __headersGoing2Hide = {COMMAND_BLOCK_ID,
                                                                  COMMAND_TYPE,
                                                                CommandBlock::AXIS_ID};
    foreach (CommandBlock::DataBaseHeaders var, __headersGoing2Hide) {
        ui->tableViewCommandBlock->hideColumn(var);
    }
    //! Toogle mode
    connect(ui->pushButtonServoOn,&QPushButton::clicked,this,&FrontManaualMode::onOperationPerformed);
    connect(ui->pushButtonAlarmClear,&QPushButton::clicked,this,&FrontManaualMode::onOperationPerformed);
}

FrontManaualMode::~FrontManaualMode()
{
    delete ui;
}
//!
//! \brief FrontManaualMode::onButtonBankSetClick
//!
void FrontManaualMode::onBankOperationClicked()
{
    if(!ui->tableViewCommandBlock->selectionModel()->hasSelection())
        return;
    if(__controller->CurrentState()!=ControllerManualMode::STATE_IDLE)
        return;

    auto button = qobject_cast<QPushButton*>(sender());

    QSqlRecord __record = __commandBlockTableFront->record(SelectedBlockIndex());

    if(button==ui->pushButtonCoordinateSet)
    {
        //setup field of coordinate
        __record.setValue(QVariant::fromValue(COORD1).toString(),ui->textBrowserPositionFeedback->toPlainText().toFloat());
    }
    else if(button == ui->pushButtonParameterSet)
    {
        //setup field of parameters
        __record.setValue(QVariant::fromValue(SPEED).toString(),ui->textEditSpeedReference->toPlainText().toFloat());
        __record.setValue(QVariant::fromValue(ACC_TIME).toString(),ui->textEditAcceralationTime->toPlainText().toFloat());
        __record.setValue(QVariant::fromValue(DEC_TIME).toString(),ui->textEditDeceralationTime->toPlainText().toFloat());
        __record.setValue(QVariant::fromValue(TORQUE_LIMIT).toString(),ui->textEditTorqueLimit->toPlainText().toFloat());
    }


    //write back to model
    __commandBlockTableFront->setRecord(SelectedBlockIndex(),__record);
}

//!
//! \brief FrontManaualMode::onButtonClick
//! For
//! 1. POSI
//! 2. ZRET
//! 3. STOP
void FrontManaualMode::onManualOperationClicked()
{
    //! Command refused
    if(__controller->CurrentState()!=ControllerManualMode::STATE_IDLE)
        return;

    auto button = qobject_cast<QPushButton*>(sender());

    AbstractCommandBlock __commandBlock;

    if(button == ui->pushButtonBankExecution)
    {
        int __commandBlockId = __commandBlockTableFront->record(SelectedBlockIndex()).value(COMMAND_BLOCK_ID).toInt();
        __commandBlock =
                QVariant::fromValue(__commandBlockAdaptor->Record(__commandBlockId,AbstractSqlTableAdpater::KEY_NAMED_KEY,QVariant::fromValue(COMMAND_BLOCK_ID).toString())).value<AbstractCommandBlock>();
    }
    else
    {
        if(button == ui->pushButtonZret)
        {
            __commandBlock = ZretCommandBlock();
            __commandBlock.Value(ZretCommandBlock::OFFSET_ZRET_DIRECTION,QVariant::fromValue(ui->radioButtonForward->isChecked()));
            __commandBlock.Value(ZretCommandBlock::OFFSET_ZRET_METHOD,QVariant::fromValue(ui->comboBoxZrtMethod->view()->selectionModel()->selectedRows().first().row()));

            __commandBlock.Value(ZretCommandBlock::OFFSET_ZRET_SPEED,ui->textEditOffset->toPlainText().toFloat());
            __commandBlock.Value(ZretCommandBlock::OFFSET_ZRET_SPEED_APPROACH,ui->textEditSpeedApproach->toPlainText().toFloat());
            __commandBlock.Value(ZretCommandBlock::OFFSET_ZRET_SPEED_CREEP,ui->textEditSpeedCreep->toPlainText().toFloat());
        }
        else
        {
            __commandBlock = PosCommandBlock();

            if(button == ui->pushButtonAbsolute)
            {
                //setup coordiante
                __commandBlock.Value(COORD1,ui->textEditCoordinateAbsolute->toPlainText().toFloat());
                __commandBlock.Value(PosCommandBlock::OFFSET_POS_ABS_REL,QVariant::fromValue(true));
            }
            else if(button == ui->pushButtonStepMinus ||
                    button == ui->pushButtonStepPlus)
            {
                //setup coordiante
                __commandBlock.Value(PosCommandBlock::OFFSET_ECB_COORD1,
                                     (ui->textEditCoordinateStep->toPlainText().toFloat()*
                                     ((button==ui->pushButtonStepPlus) ? 1 : -1)));
                __commandBlock.Value(PosCommandBlock::OFFSET_POS_ABS_REL,QVariant::fromValue(false) );//relative moving mode
            }
            else if(button == ui->pushButtonFeedForward ||
                    button == ui->pushButtonFeedBackward)
            {
                //setup coordiante
                //Positive limit/Negtive limit
                __commandBlock.Value(PosCommandBlock::OFFSET_ECB_COORD1,
                                     ((button==ui->pushButtonStepPlus) ?
                                         SelectedAxisValue(AxisContextBlock::OFFSET_CONTEXT_LIMIT_PLUS) :
                                         SelectedAxisValue(AxisContextBlock::OFFSET_CONTEXT_LIMIT_MINUS)));
                __commandBlock.Value(PosCommandBlock::OFFSET_POS_ABS_REL,QVariant::fromValue(true));//Always in ABS
            }

        }//Pos

        setCommonParameters(__commandBlock);

    }//


    //! trigger the sequence
    __commitOption.Mode(CommitBlock::MODE_EXE_COMMAND_BLOCK);
    __controller->DataBlock(QVariant::fromValue(__commandBlock));
    __controller->CommitOption(__commitOption);
    emit __controller->operationTriggered();
}
//!
//! \brief FrontManaualMode::onOperationPressed
//!
void FrontManaualMode::onOperationPerformed()
{
    AxisOperationBlock __block;

    if(sender()==ui->pushButtonServoOn)
    {
        __block.Operation(AxisMonitorBlock::OP_SERVO_ON,true);
    }
    else if(sender()==ui->pushButtonAlarmClear)
    {
        __block.Operation(AxisMonitorBlock::OP_ALARM_CLEAR,true);
    }

    __commitOption.Mode(CommitBlock::MODE_EXE_AXIS);
    __controller->CommitOption(__commitOption);
    __controller->DataBlock(QVariant::fromValue(__block));
    emit __controller->operationTriggered();
}

void FrontManaualMode::setCommonParameters(AbstractCommandBlock& __commandBlock)
{
    //!
    //! Setup common parameters
    //! TODO , unsafe input
    __commandBlock.Value(CommandBlock::AXIS_ID,SelectedAxisValue(QVariant::fromValue(CommandBlock::AXIS_ID)));

    __commandBlock.Value(SPEED,ui->textEditSpeedReference->toPlainText().toFloat());//unit/sec
    __commandBlock.Value(ACC_TIME,ui->textEditAcceralationTime->toPlainText().toFloat());//ms
    __commandBlock.Value(DEC_TIME,ui->textEditDeceralationTime->toPlainText().toFloat());//ms
    __commandBlock.Value(TORQUE_LIMIT,ui->textEditTorqueLimit->toPlainText().toFloat());// 0.01%

}

void FrontManaualMode::onTimerTimeout()
{
    AxisMonitorBlock amb = QVariant::fromValue(__controller->MonitorBlock()).value<AxisMonitorBlock>();

    ui->textBrowserPositionReference->setText(QString::number(amb.PositionCommand()));
    ui->textBrowserPositionFeedback->setText(QString::number(amb.PositionFeedback()));
    ui->textBrowserSpeedFeedback->setText(QString::number(amb.SpeedFeedback()));
    ui->textBrowserTorqueFeedback->setText(QString::number(amb.TorqueFeedback()));

    //! Servo On/Alarm clear
    utilities::colorChangeOver(ui->pushButtonServoOn, amb.Operation(AxisMonitorBlock::OP_SERVO_ON));
    utilities::colorChangeOver(ui->pushButtonAlarmClear, amb.Operation(AxisMonitorBlock::OP_ALARM_CLEAR),Qt::red);
    //! Servo ready/alarm
    utilities::colorChangeOver(ui->labelServoReady,amb.RunStatus(AxisMonitorBlock::RS_SERVO_READY));
    utilities::colorChangeOver(ui->labelAlarm,amb.Warning() > 0 || amb.Alarm() > 0,Qt::red);
    ui->textBrowserWarning->setText(QString(amb.Warning()));
    ui->textBrowseAlarm->setText(QString(amb.Alarm()));
}

int FrontManaualMode::SelectedBlockIndex() const
{
    return ui->tableViewCommandBlock->selectionModel()->selectedRows().first().row();
}
QVariant FrontManaualMode::SelectedAxisValue(QVariant keyName) const
{
    return utilities::getSqlTableSelectedRecord(__axisTableFront,QVariant::fromValue(AxisBlock::AXIS_ID),
                                                QVariant::fromValue(__selectedAxisId)).value(keyName.toString());
}

void FrontManaualMode::showEvent(QShowEvent *event)
{
    __controller->MonitorDeviceCategrory(CommitBlock::SELECTION_AXIS);
    __controller->onMonitorDeviceIndexChanged(SelectedAxisValue(QVariant::fromValue(AxisBlock::AXIS_ID)).value<MODBUS_WORD>());
    //!
    QWidget::showEvent(event);
}
