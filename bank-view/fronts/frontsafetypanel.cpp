#include "frontsafetypanel.h"
#include "ui_frontsafetypanel.h"
#include <QDebug>


FrontSafetyPanel::FrontSafetyPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrontSafetyPanel)
{
    ui->setupUi(this);
    //!
//    __controller = ControllerSafety::Instance();
//    connect(__controller,&ControllerSafety::initialized,this,&FrontSafetyPanel::onInitialized);
    //! Monitor
//    qDebug() << __monitorTable->rowCount();
//    __monitorTable->setFilter(utilities::generateFilterString(QVariant::fromValue(IoAttributes::REGION),
//                                                              QVariant::fromValue(32))); //filter out safety related inputs
//    ui->tableViewMonitor->setModel(__monitorTable);
//    HEADER_STRUCTURE::HeaderRender::renderViewHeader(JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::HEADER_IO),
//                                                     ui->tableViewMonitor);
    //! Control
//    connect(ui->checkBoxSolenoid,&QCheckBox::toggled,__controller,&ControllerSafety::DoorSolenoid);
//    connect(ui->checkBoxInterlockBypass,&QCheckBox::toggled,__controller,&ControllerSafety::InterlockBypass);
//    connect(ui->checkBoxAirChoke,&QCheckBox::toggled,__controller,&ControllerSafety::ManualAirChoke);
//    connect(ui->checkBoxLightgateBypass,&QCheckBox::toggled,__controller,&ControllerSafety::LightGateBypass);

//    ui->checkBoxAirChoke->setChecked(true);
    //! Auth
//    __disableList[AUTH::ROLE_OPERATOR].append(ui->checkBoxInterlockBypass);
//    __disableList[AUTH::ROLE_OPERATOR].append(ui->checkBoxLightgateBypass);
//    __disableList[AUTH::ROLE_OPERATOR].append(ui->checkBoxAirChoke);
}

FrontSafetyPanel::~FrontSafetyPanel()
{
    delete ui;
}

//void FrontSafetyPanel::onInitialized()
//{
//    ui->checkBoxInterlockBypass->setChecked(__controller->Status(ControllerSafety::INTERLOCK_BYPASS));
//    ui->checkBoxSolenoid->setChecked(__controller->Status(ControllerSafety::DOOR_SOLENOID_CONTROL));
//}
