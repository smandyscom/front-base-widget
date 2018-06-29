#include "frontcontrolpanel.h"
#include "ui_frontcontrolpanel.h"

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
    //! Safety I/O monitor
    FrontSafetyPanel* fsp = new FrontSafetyPanel(ui->widgetSafetyIO);
    //!
    for(int i=0;i<materialSlots.count();i++){
        FrontSlot* __frontSlot = new FrontSlot(materialSlots[i],this);
        ui->gridLayoutMaterial->addWidget(__frontSlot,i/6,i%6);
    }
    //!
//    frontMaterialSelection* __fms = new frontMaterialSelection(ControllerMaterialTransfer::DataBase(),
//                                                               ui->frameWorking);
//    //!added for creative slot
//    connect(__fms,&frontMaterialSelection::fieldValueChanged,materialSlots[0],&ControllerMaterialTransfer::onFieldValueChaged);
//    __fms->onInitialized(); // inform material controller
//    FrontMaterialSelectionV2* __fms2 = new FrontMaterialSelectionV2(ControllerMaterialTransfer::DataBase(),
//                                                                    ui->frameWorking);

}

FrontControlPanel::~FrontControlPanel()
{
    delete ui;
}

void FrontControlPanel::onTimerTimeout()
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

    ui->lcdNumberCycleTime->display(__controller->Data(ControllerMainPanel::MON_DATA_4).toReal());

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
        break;
    case ControllerMainPanel::STATE_SEMI_AUTO:
        if(!ui->radioButtonAuto->isChecked())
            ui->radioButtonAuto->setChecked(true);
        break;
    default:
        break;
    }
}

void FrontControlPanel::onErrorChanged(bool currentError)
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


