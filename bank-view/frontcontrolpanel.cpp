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
    //! Timer activated
    __timer = new QTimer(this);
    connect(__timer,SIGNAL(timeout()),this,SLOT(onTimerTimeout()));
    __timer->start();


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
    ui->pushButtonErrorReset->setEnabled(__controller->ErrorCode()!=0);
    //!Show error
    ui->textBrowserErrorDescription->setText(QString::number(__controller->ErrorCode()));
    ui->textBrowserErrorCode->setText(QString::number(__controller->ErrorCategrory()));
    ui->textBrowserErrorPart->setText(QString::number(__controller->ErrorDeviceIndex()));
    //!TODO
}
