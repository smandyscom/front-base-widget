#include "frontmanaualmode.h"
#include "ui_frontmanaualmode.h"

FrontManaualMode::FrontManaualMode(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrontManaualMode)
{
    ui->setupUi(this);

    connect(qApp,SIGNAL(focusChanged(QWidget*,QWidget*)),this,SLOT(onFocusChanged(QWidget*,QWidget*));
}

FrontManaualMode::~FrontManaualMode()
{
    delete ui;
}

//!
//! \brief FrontManaualMode::onButtonClick
//! For
//! 1. POSI
//! 2. ZRET
//! 3. STOP
void FrontManaualMode::onButtonClick()
{\
    auto button = qobject_cast<QPushButton*>(sender());
    setCommonParameters(); //

    switch (button->objectName()) {
    case "pushButtonStop":
        //! raise immediate stop request
        emit controller->requireWriteData(AbstractAddress(ControllerManualMode::CANCEL),QVariant::fromValue(true));
        return;
        break;
    case "pushButtonZret":
    {
        ZretCommandBlock* zcb = static_cast<ZretCommandBlock*>(&genericCommandBlock);
        zcb->setDirection(ui->radioButtonForward);
        zcb->method = DEC1_C_PULSE;
        zcb->offset = ui->textEditOffset->toPlainText().toFloat() * 1000;
        zcb->speedCreep = ui->textEditSpeedCreep->toPlainText().toFloat() * 1000;
        zcb->speedAppoach = ui->textEditSpeedApproach->toPlainText().toFloat() * 1000;
        zcb->commandType = ZRET;
        break;
    }
    case "pushButtonPosition":
    {
        //setup coordiante
        PosICommandBlock* pcb = static_cast<PosICommandBlock*>(&genericCommandBlock);
        pcb->coordinate = ui->textEditCoordinate->toPlainText().toFloat() *1000;
        pcb->setAbsoluteMode(ui->radioButtonAbsolute);
        pcb->commandType = POS_I;
        break;
    }
    default:
        break;
    }

    //! trigger the sequence
    emit controller->operationTriggered();
}

void FrontManaualMode::onButtonPressed()
{
    auto button  = qobject_cast<QPushButton*>(sender());
    setCommonParameters();

    switch (button->objectName()) {
    case "pushButtonFeedForward":
        break;
    case "pushButtonFeedBackward":
        break;
    default:
        break;
    }

    //! trigger the sequence
    emit controller->operationTriggered();
}
//!
//! \brief FrontManaualMode::onButtonReleased
//! Trigger stop
void FrontManaualMode::onButtonReleased()
{
    //! raise immediate stop request
    emit controller->requireWriteData(AbstractAddress(ControllerManualMode::CANCEL),QVariant::fromValue(true));
}
void FrontManaualMode::setCommonParameters()
{
    AbstractCommandBlock* base = static_cast<AbstractCommandBlock*>(&genericCommandBlock);
    //!
    //! Setup common parameters
    //!
    base->controlWord = 0;//all reset
    //TODO , unsafe input
    //
    base->speed = ui->textEditSpeedReference->toPlainText().toFloat() * 1000; // to units/sec
    base->acceralation = ui->textEditAcceralationTime->toPlainText().toFloat() * 1000; // to ms
    base->deceralation = ui->textEditDeceralationTime->toPlainText().toFloat() * 1000; // to ms
    base->torqueLimit = ui->textEditTorqueLimit->toPlainText().toFloat() * 100; // to 0.01%

    //reset cancel request
    emit controller->requireWriteData(AbstractAddress(ControllerManualMode::CANCEL),QVariant::fromValue(false));
}


void FrontManaualMode::onAxisChanged(quint16 id)
{
    //! change base-object-id
    static_cast<AbstractCommandBlock*>(&genericCommandBlock)->objectId = id;
    //! commit axis id
    emit controller->requireWriteData(AbstractAddress(ControllerManualMode::AXIS_ADR),
                                      QVariant::fromValue(static_cast<MODBUS_WORD>(id)));
}

void FrontManaualMode::onFocusChanged(QWidget *old, QWidget *now)
{
    if(this != now && this !=old)
        return; //irrelavent signal

    //! on focused
    emit controller->requireWriteData(AbstractAddress(ControllerManualMode::ENGAGED_HMI),QVariant::fromValue(this == now));
}
