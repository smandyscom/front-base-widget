#include "frontmanaualmode.h"
#include "ui_frontmanaualmode.h"

FrontManaualMode::FrontManaualMode(QAbstractTableModel* wholeCommandBankModel,
                                   QAbstractTableModel* wholeAxisBankModel,
                                   QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrontManaualMode)
{
    ui->setupUi(this);

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
    ui->comboBoxAxisName->setModelColumn();//setup the visiable column
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
    switch (button->objectName()) {
    case "pushButtonCoordinateSet":
        emit bankCoordinateSet(ui->textEditCoordinate->toPlainText().toFloat());
        break;
    case "pushButtonParameterSet":
        emit bankParameterSet(QVariant::fromValue(__commandBlock));
        break;
    case "pushButtonBankExecution":
        //! trigger the sequence
        //!  CommandBlock had been preseted by external unit
        controller->CommandBlock(__commandBlock);
        emit controller->operationTriggered();
        break;
    default:
        break;
    }
}

//!
//! \brief FrontManaualMode::onButtonClick
//! For
//! 1. POSI
//! 2. ZRET
//! 3. STOP
void FrontManaualMode::onOperationTriggered()
{\
    auto button = qobject_cast<QPushButton*>(sender());
    setCommonParameters(); //

    switch (button->objectName()) {
    case "pushButtonZret":
    {
        ZretCommandBlock* zcb = static_cast<ZretCommandBlock*>(&__commandBlock);
        zcb->Direction(ui->radioButtonForward);
        zcb->Method(DEC1_C_PULSE);
        zcb->Offset(ui->textEditOffset->toPlainText());
        zcb->SpeedCreep(ui->textEditSpeedCreep->toPlainText().toFloat());
        zcb->SpeedAppoach(ui->textEditSpeedApproach->toPlainText().toFloat());
        zcb->CommandType(BCT_ZRET);
        break;
    }
    case "pushButtonPosition":
    {
        //setup coordiante
        PosICommandBlock* pcb = static_cast<PosICommandBlock*>(&__commandBlock);
        pcb->Coordinate(ui->textEditCoordinate->toPlainText().toFloat());
        pcb->IsAbsoluteMode(ui->radioButtonAbsolute->isChecked());
        pcb->CommandType(BCT_POS_I);
        break;
    }
    case "pushButtonFeedForward":
    case "pushButtonFeedBackward":
    {
        FeedCommandBlock* fcb = static_cast<FeedCommandBlock*>(&__commandBlock);
        fcb->Direction(button->objectName() == "pushButtonFeedForward");
        fcb->CommandType(BCT_FEED);
        break;
    }
    default:
        break;
    }

    //! trigger the sequence
    controller->CommandBlock(__commandBlock);
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
    __commandBlock->Speed(ui->textEditSpeedReference->toPlainText().toFloat()); // to units/sec
    __commandBlock->Acceralation(ui->textEditAcceralationTime->toPlainText().toFloat()); // to ms
    __commandBlock->Deceralation(ui->textEditDeceralationTime->toPlainText().toFloat()); // to ms
    __commandBlock->TorqueLimit(ui->textEditTorqueLimit->toPlainText().toFloat()); // to 0.01%

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
    const AxisMonitorBlock* amb = static_cast<const AxisMonitorBlock*>(&controller->MonitorBlock());

    ui->textBrowserPositionReference->setText(amb->PositionCommand());
    ui->textBrowserPositionFeedback->setText(amb->PositionFeedback());
    ui->textBrowserSpeedFeedback->setText(amb->SpeedFeedback());
    ui->textBrowserTorqueFeedback->setText(amb->TorqueFeedback());
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
    auto comboBox = static_cast<QComboBox*>(sender());

    switch (comboBox->objectName()) {
    case "comboBoxAxisName":
        //! inform the bank manager
        emit axisIdChanged(comboBox->model()->data(comboBox->model()->index(comboBox->currentIndex(),0)).value<quint16>());
        //! change base-object-id
        __commandBlock.ObjectId(id);
        //! change monitor axis id
        emit controller->requireWriteData(AbstractAddress(ControllerManualMode::AXIS_ADR),
                                          QVariant::fromValue(static_cast<MODBUS_WORD>(id)));
        break;
    case "comboBoxCategrory":
        //TODO ,
        //auto table = static_cast<QAbstractTableModel*>(comboBox->model());

    default:
        break;
    }
}
