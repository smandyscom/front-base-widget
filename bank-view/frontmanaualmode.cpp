#include "frontmanaualmode.h"
#include "ui_frontmanaualmode.h"

FrontManaualMode::FrontManaualMode(QAbstractTableModel* wholeCommandBankModel,
                                   QAbstractTableModel* wholeAxisBankModel,
                                   QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrontManaualMode),
    __wholeCommandBankModel(wholeCommandBankModel)
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

    connect(updateTimer,SIGNAL(timeout()),this,SLOT(onTimerTimeout()));
    updateTimer->start(100);//every 100 ms update once

    //!
    //! combo box loading
    //! setModel on combobox
    //! (AxisId,Region,AxisName)
    ui->comboBoxAxisName->setModel(wholeAxisBankModel);
    ui->comboBoxAxisName->setModelColumn(JunctionBankDatabase::ATH_NAME);//setup the visiable column
    ui->comboBoxAxisName->setView(new QListView(ui->comboBoxAxisName));

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
    auto button = qobject_cast<QPushButton*>(sender());

    if(button==ui->pushButtonCoordinateSet)
    {
        emit bankCoordinateSet(ui->textEditCoordinate->toPlainText().toFloat());
    }
    else if(button == ui->pushButtonParameterSet)
    {
        emit bankParameterSet(QVariant::fromValue(__commandBlock));
    }
    else if(button == ui->pushButtonBankExecution)
    {
        //! trigger the sequence
        //!  CommandBlock had been preseted by external unit
        controller->CommandBlock(__commandBlock);
        emit controller->operationTriggered();
    }
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
        pcb->Coordinate(ui->textEditCoordinate->toPlainText().toFloat());
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
    controller->CommandBlock(__commandBlock);
    controller->CommitOption(__commitOption);
    emit controller->operationTriggered();
}

//!
//! \brief FrontManaualMode::onButtonReleased
//! Trigger stop
void FrontManaualMode::onOperationStopped()
{
    //! raise immediate stop request
    emit controller->requireWriteData(AbstractAddress(ControllerManualMode::CANCEL),QVariant::fromValue(true));
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
    emit controller->requireWriteData(AbstractAddress(ControllerManualMode::CANCEL),QVariant::fromValue(false));
}

void FrontManaualMode::onFocusChanged(QWidget *old, QWidget *now)
{
    if(this != now && this !=old)
        return; //irrelavent signal

    //! on focused
    emit controller->requireWriteData(AbstractAddress(ControllerManualMode::ENGAGED_HMI),QVariant::fromValue(this == now));
}

void FrontManaualMode::onTimerTimeout()
{
    AbstractMonitorBlock mb = controller->MonitorBlock();
    AxisMonitorBlock* amb = static_cast<AxisMonitorBlock*>(&mb);

    ui->textBrowserPositionReference->setText(QString::number(amb->PositionCommand()));
    ui->textBrowserPositionFeedback->setText(QString::number(amb->PositionFeedback()));
    ui->textBrowserSpeedFeedback->setText(QString::number(amb->SpeedFeedback()));
    ui->textBrowserTorqueFeedback->setText(QString::number(amb->TorqueFeedback()));
}

void FrontManaualMode::onCommandBlockChanged(ExtendedCommandBlock block)
{
    __commandBlock = block;
}

//!
//! \brief FrontManaualMode::onComboBoxIndexChanged
//!
void FrontManaualMode::onComboBoxIndexChanged()
{
    auto* comboBox = static_cast<QComboBox*>(sender());

    if(comboBox == ui->comboBoxAxisName)
    {
        QModelIndex currentSelection = comboBox->model()->index(comboBox->currentIndex(),JunctionBankDatabase::ATH_ID);
        quint16 id = comboBox->model()->data(currentSelection).value<quint16>();
        //! inform the bank manager
        emit axisIdChanged(id);
        //! change base-object-id
        __commandBlock.ObjectId(id);
        //! change monitor axis id
        emit controller->requireWriteData(AbstractAddress(ControllerManualMode::AXIS_ADR),
                                          QVariant::fromValue(static_cast<MODBUS_WORD>(id)));
    }
    else if(comboBox == ui->comboBoxRegion)
    {
        //TODO ,
        //auto table = static_cast<QAbstractTableModel*>(comboBox->model());
    }

}
