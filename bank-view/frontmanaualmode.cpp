#include "frontmanaualmode.h"
#include "ui_frontmanaualmode.h"
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
FrontManaualMode::FrontManaualMode(QSqlTableModel *wholeCommandBankModel,
                                   QSqlTableModel *wholeAxisBankModel,
                                   QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrontManaualMode)
{
    ui->setupUi(this);

    //setup
    __commitOption.Mode(CommitBlock::MODE_COMMAND_BLOCK);

    //!
    //! \brief connect
    //!
    connect(qApp,SIGNAL(focusChanged(QWidget*,QWidget*)),this,SLOT(onFocusChanged(QWidget*,QWidget*)));
    connect(ui->pushButtonPosition,SIGNAL(clicked(bool)),this,SLOT(onOperationPerform()));
    connect(ui->pushButtonZret,SIGNAL(clicked(bool)),this,SLOT(onOperationPerform()));
    connect(ui->pushButtonStop,SIGNAL(clicked(bool)),this,SLOT(onOperationStopped()));

    connect(ui->pushButtonFeedForward,SIGNAL(pressed()),this,SLOT(onOperationPerform()));
    connect(ui->pushButtonFeedBackward,SIGNAL(pressed()),this,SLOT(onOperationPerform()));
    connect(ui->pushButtonFeedForward,SIGNAL(released()),this,SLOT(onOperationStopped()));
    connect(ui->pushButtonFeedBackward,SIGNAL(released()),this,SLOT(onOperationStopped()));

    connect(ui->pushButtonCoordinateSet,SIGNAL(clicked(bool)),this,SLOT(onBankOperationPerformed()));
    connect(ui->pushButtonParameterSet,SIGNAL(clicked(bool)),this,SLOT(onBankOperationPerformed()));
    connect(ui->pushButtonBankExecution,SIGNAL(clicked(bool)),this,SLOT(onBankOperationPerformed()));

    connect(ui->pushButtonSubmit,SIGNAL(clicked(bool)),this,SLOT(onSubmitted()));

    //!
    __timer = new QTimer(this);
    connect(__timer,SIGNAL(timeout()),this,SLOT(onTimerTimeout()));
    __timer->start(100);//every 100 ms update once

    //!
    //! combo box loading
    //! setModel on combobox
    //! (AxisId,Region,AxisName)
    ui->comboBoxAxisName->setModel(wholeAxisBankModel);
    ui->comboBoxAxisName->setModelColumn(JunctionBankDatabase::ATH_NAME);//setup the visiable column
    ui->comboBoxAxisName->setView(new QTableView(ui->comboBoxAxisName));

    ui->tableViewCommandBlock->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableViewCommandBlock->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableViewCommandBlock->setModel(wholeCommandBankModel);

    //decorated instance
    __commandBlockTable = new TableModelCommandBlock(wholeCommandBankModel);
    //!
    connect(ui->comboBoxAxisName,SIGNAL(currentIndexChanged(int)),this,SLOT(onComboBoxIndexChanged()));
    connect(ui->comboBoxRegion,SIGNAL(currentIndexChanged(int)),this,SLOT(onComboBoxIndexChanged()));
    ui->comboBoxAxisName->setCurrentIndex(0);
    //! Link
    __controller = ControllerManualMode::Instance();
    __bankTransfer=new ControllerBankTransfer(qobject_cast<TableModelCommandBlock*>(wholeCommandBankModel),this);
}

FrontManaualMode::~FrontManaualMode()
{
    delete ui;
}
//!
//! \brief FrontManaualMode::onButtonBankSetClick
//!
void FrontManaualMode::onBankOperationPerformed()
{
    if(!ui->tableViewCommandBlock->selectionModel()->hasSelection())
        return;

    auto button = qobject_cast<QPushButton*>(sender());

     //from model to ExtendCommandBlock
    __commandBlock = __commandBlockTable->Row(SelectedRowIndex());

    if(button==ui->pushButtonCoordinateSet)
    {
        //setup field of coordinate
        __commandBlock.Coordinate1(ui->textEditCoordinate->toPlainText().toFloat());
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
        __controller->CommandBlock(__commandBlock);
        __controller->CommitOption(__commitOption);
        emit __controller->operationTriggered();
    }

    //write back to model
    __commandBlockTable->Row(SelectedRowIndex(),__commandBlock);
}

//!
//! \brief FrontManaualMode::onButtonClick
//! For
//! 1. POSI
//! 2. ZRET
//! 3. STOP
void FrontManaualMode::onOperationPerform()
{
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
    else if(button == ui->pushButtonPosition)
    {
        //setup coordiante
        PosICommandBlock* pcb = static_cast<PosICommandBlock*>(&__commandBlock);
        pcb->Coordinate1(ui->textEditCoordinate->toPlainText().toFloat());
        pcb->IsAbsoluteMode(ui->radioButtonAbsolute->isChecked());
        pcb->CommandType(BCT_POS_I);
    }
    else if(button == ui->pushButtonFeedForward ||
            button == ui->pushButtonFeedBackward)
    {
        FeedCommandBlock* fcb = static_cast<FeedCommandBlock*>(&__commandBlock);
        fcb->Direction(button->objectName() == "pushButtonFeedForward");
        fcb->CommandType(BCT_FEED);
    }

    //! trigger the sequence
    __controller->CommandBlock(__commandBlock);
    __controller->CommitOption(__commitOption);
    emit __controller->operationTriggered();
}

//!
//! \brief FrontManaualMode::onButtonReleased
//! Trigger stop
void FrontManaualMode::onOperationStopped()
{
    //! raise immediate stop request
    emit __controller->requireWriteData(ModbusDriverAddress(ControllerManualMode::CANCEL),QVariant::fromValue(true));
}
void FrontManaualMode::setCommonParameters()
{
    //!
    //! Setup common parameters
    //! TODO , unsafe input
    __commandBlock.Speed(ui->textEditSpeedReference->toPlainText().toFloat()); // to units/sec
    __commandBlock.Acceralation(ui->textEditAcceralationTime->toPlainText().toFloat()); // to ms
    __commandBlock.Deceralation(ui->textEditDeceralationTime->toPlainText().toFloat()); // to ms
    __commandBlock.TorqueLimit(ui->textEditTorqueLimit->toPlainText().toFloat()); // to 0.01%

    //reset cancel request
    emit __controller->requireWriteData(ModbusDriverAddress(ControllerManualMode::CANCEL),QVariant::fromValue(false));
}

void FrontManaualMode::onFocusChanged(QWidget *old, QWidget *now)
{
    if(this != now && this !=old)
        return; //irrelavent signal

    //! on focused
    emit __controller->requireWriteData(ModbusDriverAddress(ControllerManualMode::ENGAGED_HMI),QVariant::fromValue(this == now));
}

void FrontManaualMode::onTimerTimeout()
{
    AbstractMonitorBlock mb = __controller->MonitorBlock();
    AxisMonitorBlock* amb = static_cast<AxisMonitorBlock*>(&mb);

    ui->textBrowserPositionReference->setText(QString::number(amb->PositionCommand()));
    ui->textBrowserPositionFeedback->setText(QString::number(amb->PositionFeedback()));
    ui->textBrowserSpeedFeedback->setText(QString::number(amb->SpeedFeedback()));
    ui->textBrowserTorqueFeedback->setText(QString::number(amb->TorqueFeedback()));

    //! raise reading requirement
    __controller->requireReadData(ModbusDriverAddress(ControllerManualMode::POS_COMMAND),QVariant::fromValue(mb));
}

//!
//! \brief FrontManaualMode::onComboBoxIndexChanged
//!
void FrontManaualMode::onComboBoxIndexChanged()
{
    auto* comboBox = static_cast<QComboBox*>(sender());

    if(comboBox == ui->comboBoxAxisName)
    {
        QModelIndex currentSelectionId = comboBox->model()->index(comboBox->currentIndex(),JunctionBankDatabase::ATH_ID);
        QModelIndex currentSelectionAddress = comboBox->model()->index(comboBox->currentIndex(),JunctionBankDatabase::ATH_ADDRESS);
        MODBUS_WORD id = comboBox->model()->data(currentSelectionId).value<MODBUS_WORD>();
        MODBUS_WORD address = comboBox->model()->data(currentSelectionAddress).value<MODBUS_WORD>();
        auto ref= ui->tableViewCommandBlock->model();
        QSqlTableModel* model = qobject_cast<QSqlTableModel*>(ui->tableViewCommandBlock->model());
        //! filter out
        model->setFilter(tr("AXIS_ID=%1").arg(address));
        //! change base-object-id
        __commandBlock.ObjectId(address);
        //! change monitor axis id
        emit __controller->requireWriteData(ModbusDriverAddress(ControllerManualMode::AXIS_ADR),
                                          QVariant::fromValue(static_cast<MODBUS_WORD>(address)));
    }
    else if(comboBox == ui->comboBoxRegion)
    {
        //TODO ,
        //auto table = static_cast<QAbstractTableModel*>(comboBox->model());
    }

}

int FrontManaualMode::SelectedRowIndex() const
{
    return ui->tableViewCommandBlock->selectionModel()->selectedRows().first().row();
}
//!
//! \brief FrontManaualMode::onSubmitted
//! Commit unstaged records(Rows) into Model and underlying device(e.g PLC)
void FrontManaualMode::onSubmitted()
{
    //! Commit to database firstly , once fail (could not pass the contraint
    //!  no need go further
    auto table = qobject_cast<QSqlTableModel*>(ui->tableViewCommandBlock->model());
    table->database().transaction();
    if(table->submitAll())
    {
        table->database().commit();
        //! Start bank trunsation
        //__bankTransfer->onTransferData(CommitBlock::MODE_DOWNLOAD);
    }
    else
    {
        qDebug() << table->lastError().text();
        table->revertAll();
        table->database().rollback();
    }
}
void FrontManaualMode::onTransfer()
{

}
