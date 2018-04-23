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
    __commandBlockAdaptor = ControllerBankTransfer::Instance()->Adaptor(CommitBlock::SELECTION_COMMAND_BLOCK);
    //! Resued widgets
    new FrontBankTransfer(CommitBlock::SELECTION_COMMAND_BLOCK,ui->widgetBankTransfer);
    FrontTwinFilter* __ftf = new FrontTwinFilter(wholeCommandBankModel,
                                                QVariant::fromValue(CommandBlock::AXIS_ID),
                                                wholeAxisBankModel,
                                                QVariant::fromValue(AxisBlock::REGION),
                                                regionModel,
                                                ui->widgetFilter);
    connect(__ftf,&FrontTwinFilter::primarySelected,[=](QVariant key){
        __selectedAxisId = key.value<MODBUS_U_WORD>();
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

    connect(ui->pushButtonBankExecution,SIGNAL(clicked(bool)),this,SLOT(onManualOperationClicked()));

    connect(ui->pushButtonCoordinateSet,SIGNAL(clicked(bool)),this,SLOT(onBankOperationClicked()));
    connect(ui->pushButtonParameterSet,SIGNAL(clicked(bool)),this,SLOT(onBankOperationClicked()));
    //!
    __timer = new QTimer(this);
    connect(__timer,SIGNAL(timeout()),this,SLOT(onTimerTimeout()));
    __timer->start(100);//every 100 ms update once

    ui->tableViewCommandBlock->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableViewCommandBlock->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableViewCommandBlock->setModel(__commandBlockTableFront);
    QList<CommandBlock::DataBaseHeaders> __headersGoing2Hide = {CommandBlock::ID,
                                                                CommandBlock::COMMAND_TYPE,
                                                                CommandBlock::AXIS_ID};
    foreach (CommandBlock::DataBaseHeaders var, __headersGoing2Hide) {
        ui->tableViewCommandBlock->hideColumn(var);
    }
    //! Toogle mode
    connect(ui->pushButtonServoOn,&QPushButton::clicked,this,&FrontManaualMode::onOperationPerformed);
    connect(ui->pushButtonAlarmClear,&QPushButton::clicked,this,&FrontManaualMode::onOperationPerformed);

    //! Map
    __browserMap[ui->textBrowserPositionReference] = AxisMonitorBlock::OFFSET_MONITOR_POS_COMMAND;
    __browserMap[ui->textBrowserPositionFeedback] = AxisMonitorBlock::OFFSET_MONITOR_POS_FEEDBACK;
    __browserMap[ui->textBrowserSpeedFeedback] = AxisMonitorBlock::OFFSET_MONITOR_SPD_FEEDBACK;
    __browserMap[ui->textBrowserTorqueFeedback] = AxisMonitorBlock::OFFSET_MONITOR_TOR_FEEDBACK;
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
        if(!ui->tableViewCommandBlock->selectionModel()->hasSelection())
            return;
        int __commandBlockId = __commandBlockTableFront->record(SelectedBlockIndex()).value(QVariant::fromValue(ID).toString()).toInt();
        *static_cast<AbstractDataBlock*>(&__commandBlock) =
                __commandBlockAdaptor->Record(__commandBlockId,
                                              AbstractSqlTableAdpater::KEY_NAMED_KEY,
                                              QVariant::fromValue(CommandBlock::ID));
    }
    else
    {
        if(button == ui->pushButtonZret)
        {
            ZretCommandBlock __zcb;

            __zcb.Value(ZretCommandBlock::OFFSET_ZRET_DIRECTION,QVariant::fromValue(ui->radioButtonForward->isChecked()));
            __zcb.Value(ZretCommandBlock::OFFSET_ZRET_METHOD,QVariant::fromValue(ui->comboBoxZrtMethod->view()->selectionModel()->selectedRows().first().row()));

            __zcb.Value(ZretCommandBlock::OFFSET_ZRET_SPEED,ui->textEditOffset->toPlainText().toFloat());
            __zcb.Value(ZretCommandBlock::OFFSET_ZRET_SPEED_APPROACH,ui->textEditSpeedApproach->toPlainText().toFloat());
            __zcb.Value(ZretCommandBlock::OFFSET_ZRET_SPEED_CREEP,ui->textEditSpeedCreep->toPlainText().toFloat());

            __commandBlock = static_cast<AbstractCommandBlock>(__zcb);
        }
        else
        {
            PosCommandBlock __pcb;

            if(button == ui->pushButtonAbsolute)
            {
                //setup coordiante
                __pcb.Value(COORD1,ui->textEditCoordinateAbsolute->toPlainText().toFloat());
                __pcb.Value(PosCommandBlock::OFFSET_POS_ABS_REL,QVariant::fromValue(true));
            }
            else if(button == ui->pushButtonStepMinus ||
                    button == ui->pushButtonStepPlus)
            {
                //setup coordiante
                __pcb.Value(PosCommandBlock::OFFSET_ECB_COORD1,
                                     (ui->textEditCoordinateStep->toPlainText().toFloat()*
                                     ((button==ui->pushButtonStepPlus) ? 1 : -1)));
                __pcb.Value(PosCommandBlock::OFFSET_POS_ABS_REL,QVariant::fromValue(false) );//relative moving mode
            }
            else if(button == ui->pushButtonFeedForward ||
                    button == ui->pushButtonFeedBackward)
            {
                //setup coordiante
                //Positive limit/Negtive limit
                __pcb.Value(PosCommandBlock::OFFSET_ECB_COORD1,
                                     ((button==ui->pushButtonFeedForward) ?
                                         SelectedAxisValue(QVariant::fromValue(AxisBlock::LIMIT_PLUS)) :
                                         SelectedAxisValue(QVariant::fromValue(AxisBlock::LIMIT_MINUS))));
                __pcb.Value(PosCommandBlock::OFFSET_POS_ABS_REL,QVariant::fromValue(true));//Always in ABS
            }

            __commandBlock = static_cast<AbstractCommandBlock>(__pcb);

        }//Pos


        setCommonParameters(__commandBlock);

    }//
    //! trigger the sequence
    __commitOption.Mode(CommitBlock::MODE_EXE_COMMAND_BLOCK);
    __controller->DataBlock(QVariant::fromValue(static_cast<CellDataBlock>(__commandBlock)));
    __controller->CommitOption(__commitOption);
    emit __controller->operationTriggered();
}
//!
//! \brief FrontManaualMode::onOperationPressed
//!
void FrontManaualMode::onOperationPerformed()
{
    AxisOperationBlock __block;
    ModbusDriverAddress __address;
    __address.setRegisterAddress(AxisOperationBlock::OFFSET_OPEATION_OPERATION);

    if(sender()==ui->pushButtonServoOn)
    {
        __address.setBitIndex(AxisMonitorBlock::OP_SERVO_ON);
    }
    else if(sender()==ui->pushButtonAlarmClear)
    {
        __address.setBitIndex(AxisMonitorBlock::OP_ALARM_CLEAR);
    }

    __block.Value(__address.getAddress(),true);
    __commitOption.Mode(CommitBlock::MODE_EXE_AXIS);
    __controller->CommitOption(__commitOption);
    __controller->DataBlock(QVariant::fromValue(static_cast<CellDataBlock>(__block)));
    emit __controller->operationTriggered();
}

void FrontManaualMode::setCommonParameters(AbstractCommandBlock& __commandBlock)
{
    //!
    //! Setup common parameters
    //! TODO , unsafe input
    //! Set Address
    __commandBlock.Value(AbstractCommandBlock::OFFSET_ACB_AXIS_ID,
                         utilities::getSqlTableSelectedRecord(__axisTableFront,
                                                              QVariant::fromValue(ID),
                                                              __selectedAxisId)
                         .value(utilities::trimNamespace(QVariant::fromValue(AxisBlock::ADDRESS))));

    __commandBlock.Value(SPEED,ui->textEditSpeedReference->toPlainText().toFloat());//unit/sec
    __commandBlock.Value(ACC_TIME,ui->textEditAcceralationTime->toPlainText().toFloat());//ms
    __commandBlock.Value(DEC_TIME,ui->textEditDeceralationTime->toPlainText().toFloat());//ms
    __commandBlock.Value(TORQUE_LIMIT,ui->textEditTorqueLimit->toPlainText().toFloat());// 0.01%

}

void FrontManaualMode::onTimerTimeout()
{
    AxisMonitorBlock amb;
    *static_cast<CellDataBlock*>(&amb) = __controller->MonitorBlock();

    foreach (QWidget* var, __browserMap.keys()) {
        qobject_cast<QTextBrowser*>(var)->setText(QString::number(amb.Value(__browserMap[var]).toReal()));
    }
    //! Servo On/Alarm clear
    ModbusDriverAddress __address;
    __address.setRegisterAddress(AxisMonitorBlock::OFFSET_MONITOR_OPERATION);

    __address.setBitIndex(AxisMonitorBlock::OP_SERVO_ON);
    utilities::colorChangeOver(ui->pushButtonServoOn, amb.Value(__address.getAddress()).toBool());
    __address.setBitIndex(AxisMonitorBlock::OP_ALARM_CLEAR);
    utilities::colorChangeOver(ui->pushButtonAlarmClear, amb.Value(__address.getAddress()).toBool(),Qt::red);
    //! Servo ready/alarm
    __address.setRegisterAddress(AxisMonitorBlock::OFFSET_MONITOR_RUN_STATUS);
    __address.setBitIndex(AxisMonitorBlock::RS_SERVO_READY);
    utilities::colorChangeOver(ui->labelServoReady,amb.Value(__address.getAddress()).toBool(),Qt::red);
    //! Alarm/Warning details
    //ui->textBrowserWarning->setText(QString(amb.Warning()));
    //ui->textBrowseAlarm->setText(QString(amb.Alarm()));
}

int FrontManaualMode::SelectedBlockIndex() const
{
    return ui->tableViewCommandBlock->selectionModel()->selectedRows().first().row();
}
QVariant FrontManaualMode::SelectedAxisValue(QVariant keyName) const
{
    return utilities::getSqlTableSelectedRecord(__axisTableFront,QVariant::fromValue(AxisBlock::ID),
                                                QVariant::fromValue(__selectedAxisId))
            .value(utilities::trimNamespace(keyName));
}

void FrontManaualMode::showEvent(QShowEvent *event)
{
    __controller->MonitorDeviceCategrory(CommitBlock::SELECTION_AXIS);
    __controller->onMonitorDeviceIndexChanged(__selectedAxisId);
    //!
    QWidget::showEvent(event);
}
