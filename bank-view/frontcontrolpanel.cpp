#include "frontcontrolpanel.h"
#include "ui_frontcontrolpanel.h"

frontControlPanel::frontControlPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::frontControlPanel)
{
    ui->setupUi(this);
    //! link
    __controller = ControllerMainPanel::Instance();
    //!Button link
    connect(ui->pushButtonPause,&QPushButton::clicked,__controller,&ControllerMainPanel::Pause);
    connect(ui->pushButtonInitialize,&QPushButton::clicked,__controller,&ControllerMainPanel::Initialize);
    connect(ui->pushButtonErrorReset,&QPushButton::clicked,__controller,&ControllerMainPanel::ErrorReset);
    connect(ui->pushButtonErrorIgnore,&QPushButton::clicked,__controller,&ControllerMainPanel::ErrorIgnore);
    connect(ui->pushButtonClear,&QPushButton::clicked,__controller,&ControllerMainPanel::Clear);

    //! Timer activated
    __timer = new QTimer(this);
    connect(__timer,SIGNAL(timeout()),this,SLOT(onTimerTimeout()));
    __timer->start();
    //!
    __manualInterlock = {
        ui->pushButtonClear,
        ui->pushButtonInitialize,
        ui->pushButtonPause
    };
    __autoInterlock = {
        ui->radioButtonAuto,
        ui->radioButtonManual,
    };
    //!
    //connect(ui->radioButtonAuto,SIGNAL(toggled(bool)),this,SLOT(onCheckedChanged()));
    connect(ui->radioButtonManual,&QRadioButton::toggled,this,&frontControlPanel::onCheckedChanged);
    //!
    connect(__controller,&ControllerMainPanel::stateChanged,this,&frontControlPanel::onStateChanged);
    connect(__controller,&ControllerMainPanel::errorChanged,this,&frontControlPanel::onErrorChanged);
    //!
    connect(__controller->ControllerTransfer(),&ControllerBankTransfer::dataTransfering,[=](TransferTask task){
        //!
        ui->progressBarSync->setValue(ui->progressBarSync->value()+1);
    });
}

frontControlPanel::~frontControlPanel()
{
    delete ui;
}

void frontControlPanel::onTimerTimeout()
{
    //!Update
    utilities::colorChangeOver(ui->pushButtonPause,__controller->IsPause(),Qt::red,Qt::green);
    ui->pushButtonInitialize->setEnabled(__controller->IsNotInitializing());
    ui->pushButtonClear->setEnabled(!__controller->IsClear());
    //!
    foreach (QWidget* var, __manualInterlock) {
        var->setEnabled(__controller->CurrentState() != ControllerMainPanel::STATE_MANUAL);
    }
    foreach (QWidget* var, __autoInterlock) {
        var->setEnabled(__controller->CurrentState() != ControllerMainPanel::STATE_AUTO);
    }
    //!
    ui->lcdNumberSyncCounter->display(__controller->ControllerTransfer()->RestTasksCount());
    //! Show aux datas
    //! InletCount
    //! OutletCount
    //! OKCount
    //! NGCount
    //! CYCTIME
    ui->lcdNumberInletCount->display(__controller->Data(ControllerMainPanel::MON_DATA_0).toInt());
    ui->lcdNumberOutletCount->display(__controller->Data(ControllerMainPanel::MON_DATA_1).toInt());
    ui->lcdNumberOKCount->display(__controller->Data(ControllerMainPanel::MON_DATA_2).toInt());
    ui->lcdNumberNgCount->display(__controller->Data(ControllerMainPanel::MON_DATA_3).toInt());

    ui->lcdNumberCycleTime->display(__controller->Data(ControllerMainPanel::MON_DATA_4).toReal());

}

void frontControlPanel::onCheckedChanged(bool toggled)
{
//    if(ui->radioButtonManual->isChecked())
//        __controller->Manual(true);
//    else if(ui->radioButtonAuto->isChecked())
        __controller->Manual(toggled);
    //!
    ui->progressBarSync->setMaximum(__controller->ControllerTransfer()->RestTasksCount());
    ui->progressBarSync->reset();
}

void frontControlPanel::onStateChanged(ControllerMainPanel::MainStates currentState)
{
    switch (currentState) {
    case ControllerMainPanel::STATE_MANUAL:
        if(!ui->radioButtonManual->isChecked())
            ui->radioButtonManual->setChecked(true);
        break;
    case ControllerMainPanel::STATE_SEMI_AUTO:
        if(!ui->radioButtonAuto->isChecked())
            ui->radioButtonAuto->setChecked(true);
        break;
    default:
        break;
    }
}

void frontControlPanel::onErrorChanged(bool currentError)
{
    //!enable buttons
    ui->pushButtonErrorReset->setEnabled(currentError);
    ui->pushButtonErrorIgnore->setEnabled(currentError &&
                                          (__controller->ErrorCategrory() == CommitBlock::SELECTION_CYLINDER));
    //! given message
    if(currentError)
        ui->textBrowserErrorDescription->setText(QString("%1\n%2")
                                                 .arg(__controller->ErrorDevice())
                                                 .arg(__controller->ErrorDescription()));
    else
        ui->textBrowserErrorDescription->clear();
}
