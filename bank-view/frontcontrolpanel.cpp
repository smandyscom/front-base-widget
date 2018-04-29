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
    connect(ui->pushButtonErrorReset,&QPushButton::clicked,[=](){
       ui->textBrowserErrorDescription->clear();
       ui->pushButtonErrorReset->setEnabled(false);
    });
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
    connect(ui->radioButtonAuto,SIGNAL(toggled(bool)),this,SLOT(onCheckedChanged()));
    connect(ui->radioButtonManual,SIGNAL(toggled(bool)),this,SLOT(onCheckedChanged()));
    //!
    connect(__controller,&ControllerMainPanel::stateChanged,this,&frontControlPanel::onStateChanged);
}

frontControlPanel::~frontControlPanel()
{
    delete ui;
}

void frontControlPanel::onTimerTimeout()
{
    //!Update
    utilities::colorChangeOver(ui->pushButtonPause,__controller->IsPause(),Qt::yellow);
    ui->pushButtonInitialize->setEnabled(__controller->IsInitialized());
    if(__controller->IsError() && !ui->pushButtonErrorReset->isEnabled())
        ui->textBrowserErrorDescription->setText(QString("%1\n%2")
                                                 .arg(__controller->ErrorDevice())
                                                 .arg(__controller->ErrorDescription()));
    ui->pushButtonErrorReset->setEnabled(__controller->IsError());

    //!
    foreach (QWidget* var, __manualInterlock) {
        var->setEnabled(__controller->CurrentState() != ControllerMainPanel::STATE_MANUAL);
    }
    foreach (QWidget* var, __autoInterlock) {
        var->setEnabled(__controller->CurrentState() != ControllerMainPanel::STATE_AUTO);
    }
}

void frontControlPanel::onCheckedChanged()
{
    __controller->Manual(ui->radioButtonManual->isChecked());
}

void frontControlPanel::onStateChanged(ControllerMainPanel::MainStates currentState)
{
    if(!ui->radioButtonManual->isChecked() &&
            currentState==ControllerMainPanel::STATE_MANUAL)
        ui->radioButtonManual->setChecked(true);
}
