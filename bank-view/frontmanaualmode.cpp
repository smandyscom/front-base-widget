#include "frontmanaualmode.h"
#include "ui_frontmanaualmode.h"
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QPalette>
#include <utilities.h>
FrontManaualMode::FrontManaualMode(AbstractQVariantSqlTable *wholeCommandBankModel,
                                   QSqlRelationalTableModel *wholeAxisBankModel,
                                   QSqlRelationalTableModel *regionModel,
                                   QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrontManaualMode)
{
    ui->setupUi(this);
    //!decorated instance
    __commandBlockTable = qobject_cast<TableModelCommandBlock*>(wholeCommandBankModel);
    __axisTable = new TableModelAxis(wholeAxisBankModel);
    __regionTable = regionModel;
    //! Link
    __controller = ControllerManualMode::Instance();
    __bankTransfer= ControllerBankTransfer::Instance();
    //setup
    __commitOption.Mode(CommitBlock::MODE_COMMAND_BLOCK);

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

    connect(ui->pushButtonSubmit,SIGNAL(clicked(bool)),this,SLOT(onDataTransfer()));
    connect(ui->pushButtonUpdate,SIGNAL(clicked(bool)),this,SLOT(onDataTransfer()));

    connect(ui->pushButtonSelectAllAxis,SIGNAL(clicked(bool)),this,SLOT(onSelectReset()));
    connect(ui->pushButtonSelectAllRegion,SIGNAL(clicked(bool)),this,SLOT(onSelectReset()));
    //!
    __timer = new QTimer(this);
    connect(__timer,SIGNAL(timeout()),this,SLOT(onTimerTimeout()));
    __timer->start(100);//every 100 ms update once

    //!
    //! combo box loading
    //! setModel on combobox
    //! (AxisId,Region,AxisName)
    ui->comboBoxAxisName->setModel(wholeAxisBankModel);
    ui->comboBoxAxisName->setModelColumn(TableModelAxis::NAME);//setup the visiable column
    QTableView* qtv = new QTableView(ui->comboBoxAxisName);
    ui->comboBoxAxisName->setView(qtv);
    qtv->setStyleSheet(ui->comboBoxAxisName->styleSheet());
    qtv->setFont(ui->comboBoxAxisName->font());
    qtv->setCornerButtonEnabled(false);

    ui->comboBoxRegion->setModel(regionModel);
    ui->comboBoxRegion->setModelColumn(1);
    qtv = new QTableView(ui->comboBoxRegion);
    ui->comboBoxRegion->setView(qtv);
    qtv->setStyleSheet(ui->comboBoxRegion->styleSheet());
    qtv->setFont(ui->comboBoxRegion->font());
    qtv->setCornerButtonEnabled(false);
    qtv->hideColumn(0);

    ui->tableViewCommandBlock->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableViewCommandBlock->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableViewCommandBlock->setModel(__commandBlockTable);
    QList<TableModelCommandBlock::Headers> __headersGoing2Hide = {TableModelCommandBlock::COMMAND_BLOCK_ID,
                                                                 TableModelCommandBlock::COMMAND_TYPE,
                                                                 TableModelCommandBlock::AXIS_ID};
    foreach (TableModelCommandBlock::Headers var, __headersGoing2Hide) {
        ui->tableViewCommandBlock->hideColumn(var);
    }

    //!
    connect(ui->comboBoxAxisName,SIGNAL(currentIndexChanged(int)),this,SLOT(onComboBoxIndexChanged()));
    connect(ui->comboBoxRegion,SIGNAL(currentIndexChanged(int)),this,SLOT(onComboBoxIndexChanged()));
    ui->comboBoxAxisName->setCurrentIndex(0);
    ui->comboBoxRegion->setCurrentIndex(0);

    //! Progress bar
    ui->progressBarDataTransfer->setMinimum(0);
    connect(__controller,&ControllerManualMode::operationPerformed,this,[this](){
        if(__controller->CommitOption().Mode() == CommitBlock::MODE_COMMAND_BLOCK)
            return; //ignorance
        ui->progressBarDataTransfer->setValue(__bankTransfer->CurrentIndex()+1);
    });

    //! Toogle mode
    connect(ui->pushButtonServoOn,&QPushButton::clicked,this,&FrontManaualMode::onOperationPerformed);
    //! Press/Release mode
    connect(ui->pushButtonAlarmClear,&QPushButton::pressed,this,&FrontManaualMode::onOperationPerformed);
    connect(ui->pushButtonAlarmClear,&QPushButton::released,this,&FrontManaualMode::onOperationPerformed);
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

    auto button = qobject_cast<QPushButton*>(sender());

     //from model to ExtendCommandBlock
    __commandBlock = __commandBlockTable->RowRecord(SelectedRowIndex()).value<ExtendedCommandBlock>();

    if(button==ui->pushButtonCoordinateSet)
    {
        //setup field of coordinate
        __commandBlock.Coordinate1(ui->textBrowserPositionFeedback->toPlainText().toFloat());
    }
    else if(button == ui->pushButtonParameterSet)
    {
        //setup field of parameters
        __commandBlock.Speed(ui->textEditSpeedReference->toPlainText().toFloat());
        __commandBlock.Acceralation(ui->textEditAcceralationTime->toPlainText().toFloat());
        __commandBlock.Deceralation(ui->textEditDeceralationTime->toPlainText().toFloat());
        __commandBlock.TorqueLimit(ui->textEditTorqueLimit->toPlainText().toFloat());
    }
    else if(button == ui->pushButtonBankExecution)
    {
        __controller->DataBlock(QVariant::fromValue(__commandBlock));
        __controller->CommitOption(__commitOption);
        emit __controller->operationTriggered();
    }

    //write back to model
    __commandBlockTable->RowRecord(SelectedRowIndex(),QVariant::fromValue(__commandBlock));
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
    setCommonParameters(); //

    if(button == ui->pushButtonZret)
    {
        ZretCommandBlock* zcb = static_cast<ZretCommandBlock*>(&__commandBlock);
        zcb->Direction(ui->radioButtonForward);
        zcb->Method(DEC1_C_PULSE);
        zcb->Offset(ui->textEditOffset->toPlainText().toFloat());
        zcb->SpeedCreep(ui->textEditSpeedCreep->toPlainText().toFloat());
        zcb->SpeedAppoach(ui->textEditSpeedApproach->toPlainText().toFloat());
        zcb->CommandType(BCT_ZRET);
    }
    else if(button == ui->pushButtonAbsolute)
    {
        //setup coordiante
        PosICommandBlock* pcb = static_cast<PosICommandBlock*>(&__commandBlock);
        pcb->Coordinate1(ui->textEditCoordinateAbsolute->toPlainText().toFloat());
        pcb->IsAbsoluteMode(true);
        pcb->CommandType(BCT_POS_I);
    }
    else if(button == ui->pushButtonStepMinus ||
            button == ui->pushButtonStepPlus)
    {
        float multi=1;
        if(button==ui->pushButtonStepMinus)
            multi=-1;
        //setup coordiante
        PosICommandBlock* pcb = static_cast<PosICommandBlock*>(&__commandBlock);
        pcb->Coordinate1(ui->textEditCoordinateStep->toPlainText().toFloat()*multi);
        pcb->IsAbsoluteMode(false);//relative moving mode
        pcb->CommandType(BCT_POS_I);
    }
    else if(button == ui->pushButtonFeedForward ||
            button == ui->pushButtonFeedBackward)
    {
        //setup coordiante
        PosICommandBlock* pcb = static_cast<PosICommandBlock*>(&__commandBlock);
        //Positive limit/Negtive limit
        if(button==ui->pushButtonFeedForward)
            pcb->Coordinate1(SelectedAxisValue(TableModelAxis::LIMIT_PLUS).toReal());
        else
            pcb->Coordinate1(SelectedAxisValue(TableModelAxis::LIMIT_MINUS).toReal());


        pcb->IsAbsoluteMode(true);//always in ABS
        pcb->CommandType(BCT_POS_I);
    }

    //! trigger the sequence
    __controller->DataBlock(QVariant::fromValue(__commandBlock));
    __controller->CommitOption(__commitOption);
    emit __controller->operationTriggered();
}
//!
//! \brief FrontManaualMode::onOperationPressed
//!
void FrontManaualMode::onOperationPerformed()
{
    ControllerManualMode::ManualContext __controlBit;
    if(sender()==ui->pushButtonServoOn)
    {
        __controlBit = ControllerManualMode::SERVO_ON;
    }
    else if(sender()==ui->pushButtonAlarmClear)
    {
        __controlBit = ControllerManualMode::ALARM_CLEAR;
    }

    __controller->Operation(__controlBit);
}

void FrontManaualMode::setCommonParameters()
{
    //!
    //! Setup common parameters
    //! TODO , unsafe input
    __commandBlock.ObjectId(SelectedAxisAddress());

    __commandBlock.Speed(ui->textEditSpeedReference->toPlainText().toFloat()); // to units/sec
    __commandBlock.Acceralation(ui->textEditAcceralationTime->toPlainText().toFloat()); // to ms
    __commandBlock.Deceralation(ui->textEditDeceralationTime->toPlainText().toFloat()); // to ms
    __commandBlock.TorqueLimit(ui->textEditTorqueLimit->toPlainText().toFloat()); // to 0.01%

    __commandBlock.ControlWord(AbstractCommandBlock::IS_PARA_SETTED,false);
    __commandBlock.ControlWord(AbstractCommandBlock::IS_RESET_POS_REFERENCE,false);


}

void FrontManaualMode::onTimerTimeout()
{
    AbstractMonitorBlock mb = __controller->MonitorBlock();
    AxisMonitorBlock* amb = static_cast<AxisMonitorBlock*>(&mb);

    ui->textBrowserPositionReference->setText(QString::number(amb->PositionCommand()));
    ui->textBrowserPositionFeedback->setText(QString::number(amb->PositionFeedback()));
    ui->textBrowserSpeedFeedback->setText(QString::number(amb->SpeedFeedback()));
    ui->textBrowserTorqueFeedback->setText(QString::number(amb->TorqueFeedback()));

    //! Servo On/Alarm clear
    utilities::colorChangeOver(ui->pushButtonServoOn, amb->Operation(AxisMonitorBlock::OP_SERVO_ON));
    utilities::colorChangeOver(ui->pushButtonAlarmClear, amb->Operation(AxisMonitorBlock::OP_ALARM_CLEAR),Qt::red);
    //! Servo ready/alarm
    utilities::colorChangeOver(ui->labelServoReady,amb->RunStatus(AxisMonitorBlock::RS_SERVO_READY));
    utilities::colorChangeOver(ui->labelAlarm,amb->Warning() > 0 || amb->Alarm() > 0,Qt::red);
    ui->textBrowserWarning->setText(QString(amb->Warning()));
    ui->textBrowseAlarm->setText(QString(amb->Alarm()));
}

//!
//! \brief FrontManaualMode::onComboBoxIndexChanged
//!
void FrontManaualMode::onComboBoxIndexChanged()
{
    auto* comboBox = static_cast<QComboBox*>(sender());

    if(comboBox == ui->comboBoxAxisName)
    {
        //! filter out
        __commandBlockTable->setFilter(tr("AXIS_ID=%2").arg(SelectedAxisAddress()));
        //! change base-object-id
        __commandBlock.ObjectId(SelectedAxisAddress());
        //! change monitor axis id
        __controller->onMonitorAxisChanged(static_cast<MODBUS_WORD>(SelectedAxisAddress()));
    }
    else if(comboBox == ui->comboBoxRegion)
    {
        QString regionKey = QVariant::fromValue(TableModelAxis::REGION).value<QString>();
        int regionId = __regionTable->index(comboBox->currentIndex(),0).data().toUInt();

        QString filterString = tr("%1=%2").arg(regionKey).arg(regionId);
        __axisTable->setFilter(filterString);
        //! reselect axis
        ui->comboBoxAxisName->setCurrentIndex(0);
    }

}

int FrontManaualMode::SelectedRowIndex() const
{
    return ui->tableViewCommandBlock->selectionModel()->selectedRows().first().row();
}
MODBUS_WORD FrontManaualMode::SelectedAxisAddress() const
{
    return ui->comboBoxAxisName->model()->index(ui->comboBoxAxisName->currentIndex(),TableModelAxis::ADDRESS).data().value<MODBUS_WORD>();
}
MODBUS_WORD FrontManaualMode::SelectedAxisId() const
{
    return ui->comboBoxAxisName->model()->index(ui->comboBoxAxisName->currentIndex(),TableModelAxis::AXIS_ID).data().value<MODBUS_WORD>();
}
QVariant FrontManaualMode::SelectedAxisValue(TableModelAxis::Headers header) const
{
    return __axisTable->Value(ui->comboBoxAxisName->currentIndex(),header);
}

//!
//! \brief FrontManaualMode::onSubmitted
//! Commit unstaged records(Rows) into Model and underlying device(e.g PLC)
void FrontManaualMode::onDataTransfer()
{
    //! Refuse
    if(__controller->CurrentState()!=ControllerManualMode::STATE_IDLE)
        return;

    auto table = qobject_cast<QSqlTableModel*>(ui->tableViewCommandBlock->model());


    if(sender()==ui->pushButtonSubmit)
    {
        //! Commit to database firstly , once fail (could not pass the contraint
        //!  no need go further
        //table->database().transaction();
        if(table->submitAll())
        {
            //table->database().commit();
            //! Start bank trunsation
            table->setFilter(nullptr); //reset filter
            __bankTransfer->DataBlockSelection(CommitBlock::SELECTION_COMMAND_BLOCK);
            __bankTransfer->Direction(CommitBlock::MODE_DOWNLOAD_DATA_BLOCK);
            __bankTransfer->onTransferData(); // mode , transfer all
            //TODOS , optimization , transfer those rows edited
        }
        else
        {
            qDebug() << table->lastError().text();
            table->revertAll();
            //table->database().rollback();
        }
    }
    else if(sender()==ui->pushButtonUpdate)
    {
        table->setFilter(""); //reset filter
        __bankTransfer->DataBlockSelection(CommitBlock::SELECTION_COMMAND_BLOCK);
        __bankTransfer->Direction(CommitBlock::MODE_UPLOAD_DATA_BLOCK);
        __bankTransfer->onTransferData();
    }

    //!Setup progress bar
    ui->progressBarDataTransfer->setMaximum(__bankTransfer->Goal());
    ui->progressBarDataTransfer->reset();
}

void FrontManaualMode::onSelectReset()
{
    if(sender()==ui->pushButtonSelectAllRegion)
    {
        __axisTable->setFilter("");
    }
    else if(sender()==ui->pushButtonSelectAllAxis)
    {
        __commandBlockTable->setFilter("");
    }
}
