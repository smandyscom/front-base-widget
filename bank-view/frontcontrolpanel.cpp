#include "frontcontrolpanel.h"
#include "ui_frontcontrolpanel.h"

#include <QDebug>

FrontControlPanel::FrontControlPanel(QList<ControllerMaterialTransfer*> materialSlots,
                                     QWidget *parent) :
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
    connect(ui->radioButtonManual,&QRadioButton::toggled,this,&FrontControlPanel::onCheckedChanged);
    //!
    connect(__controller,&ControllerMainPanel::stateChanged,this,&FrontControlPanel::onStateChanged);
    connect(__controller,&ControllerMainPanel::errorChanged,this,&FrontControlPanel::onErrorChanged);
    connect(__controller,&ControllerMainPanel::firstTimeAck,this,&FrontControlPanel::onFirstTimeAck);
    //!
    connect(ui->pushButtonInletClear,&QPushButton::clicked,this,&FrontControlPanel::onInletCleared);
    connect(ui->pushButtonETUsageClear,&QPushButton::clicked,this,&FrontControlPanel::onETCounterCleared);
    connect(ui->spinBoxETUsageAlarm,&QSpinBox::editingFinished,this,&FrontControlPanel::onETThresholdChanged);
    connect(ui->checkBoxETNgAlarm,&QCheckBox::toggled,this,&FrontControlPanel::onETNGMindChecked);
    //! Safety I/O monitor
    FrontSafetyPanel* fsp = new FrontSafetyPanel(ui->widgetSafetyIO);
    //!
    for(int i=0;i<materialSlots.count();i++){
        FrontSlot* __frontSlot = new FrontSlot(materialSlots[i],i!=0,this);
        ui->gridLayoutMaterial->addWidget(__frontSlot,i/3,i%3);
    }
    //!
    FrontMaterialSelectionV2* __fms2 = new FrontMaterialSelectionV2(ControllerMaterialTransfer::DataBase(),
                                                                    ui->frameWorking);

}

FrontControlPanel::~FrontControlPanel()
{
    delete ui;
}

void FrontControlPanel::onTimerTimeout()
{
    //!Update
    utilities::colorChangeOver(ui->pushButtonPause,__controller->IsPause(),Qt::red,Qt::green);
    ui->pushButtonInitialize->setEnabled(__controller->InitializingState() != ControllerMainPanel::INITIALING);
    ui->pushButtonClear->setEnabled(!__controller->IsClear() && __controller->InitializingState() == ControllerMainPanel::INITIALIZED);
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
    ui->lcdNumberInletCount->display(__controller->Data(ControllerMainPanel::MON_DATA_0).toInt());
    ui->lcdNumberCycleTime->display(__controller->Data(ControllerMainPanel::MON_DATA_4).toReal());
    ui->lcdNumberETUsageCounter->display(__controller->Data(ControllerMainPanel::ET_USAGE_COUNTER).toInt());
    //! Activate clear
    ui->pushButtonETUsageClear->setEnabled(ui->lcdNumberETUsageCounter->value()>=ui->spinBoxETUsageAlarm->value());
        //!
    switch (__controller->InitializingState()) {
        case ControllerMainPanel::WAIT_INITIAL:
            ui->textBrowserInitializing->setText(QString("等待復歸"));
            break;
        case ControllerMainPanel::INITIALING:
            ui->textBrowserInitializing->setText(QString("復歸中..."));
            break;
        case ControllerMainPanel::INITIALIZED:
            ui->textBrowserInitializing->setText(QString("復歸完成..."));
            break;
        default:
            break;
    }

    //!
    bool result = true;
    for(int i=2;i<7;i++)
        result &= ModbusChannel::Channels()[i]->IsConnected();
    switch (result) {
    case true:
        ui->textBrowserNetwork->setText("PLC連線正常");

        break;
    case false:
        ui->textBrowserNetwork->setText("PLC連線異常");
        break;
    default:
        break;
    }
    utilities::colorChangeOver(ui->labelNetword,result,Qt::green,Qt::red);
}

void FrontControlPanel::onCheckedChanged(bool toggled)
{
        __controller->Manual(toggled);
    //!
}

void FrontControlPanel::onStateChanged(ControllerMainPanel::MainStates currentState)
{
    switch (currentState) {
    case ControllerMainPanel::STATE_MANUAL:
        if(!ui->radioButtonManual->isChecked())
            ui->radioButtonManual->setChecked(true);

        ui->spinBoxETUsageAlarm->setEnabled(true);

        break;
    case ControllerMainPanel::STATE_SEMI_AUTO:
        if(!ui->radioButtonAuto->isChecked())
            ui->radioButtonAuto->setChecked(true);

        ui->spinBoxETUsageAlarm->setValue(__controller->Data(ControllerMainPanel::ET_USAGE_THRESHOLD).toInt());
        ui->spinBoxETUsageAlarm->setEnabled(false);
        break;
    default:

        break;
    }
}

void FrontControlPanel::onErrorChanged(MODBUS_U_QUAD currentError)
{
    //!enable buttons
    ui->pushButtonErrorReset->setEnabled(currentError!=0);
    ui->pushButtonErrorIgnore->setEnabled(currentError!=0 &&
                                          (__controller->ErrorCategrory() == CommitBlock::SELECTION_CYLINDER));
    //! given message
    if(currentError)
    {
        ui->textBrowserErrorDescription->setText(QString("%1\n%2")
                                                 .arg(__controller->ErrorDevice())
                                                 .arg(__controller->ErrorDescription()));
        qInfo() << ui->textBrowserErrorDescription->toPlainText(); //record alarm
    }
    else
        ui->textBrowserErrorDescription->clear();
}

void FrontControlPanel::onFirstTimeAck()
{
    ui->checkBoxETNgAlarm->setChecked(__controller->Data(ControllerMainPanel::ET_NG_MIND).toBool());
}

void FrontControlPanel::onETThresholdChanged()
{
    __controller->Data(ControllerMainPanel::ET_USAGE_THRESHOLD,QVariant::fromValue(ui->spinBoxETUsageAlarm->value()));
}
void FrontControlPanel::onETCounterCleared()
{
    __controller->Data(ControllerMainPanel::ET_USAGE_COUNTER,QVariant::fromValue(0));
}
void FrontControlPanel::onETNGMindChecked()
{
    __controller->Data(ControllerMainPanel::ET_NG_MIND,ui->checkBoxETNgAlarm->isChecked());
}


void FrontControlPanel::onInletCleared()
{
    __controller->Data(ControllerMainPanel::MON_DATA_0,QVariant::fromValue(0));
}
