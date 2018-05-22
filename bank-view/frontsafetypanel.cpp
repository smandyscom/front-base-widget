#include "frontsafetypanel.h"
#include "ui_frontsafetypanel.h"
#include <QDebug>


FrontSafetyPanel::FrontSafetyPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrontSafetyPanel),
    __monitorTable(new TableModelIOOverride(JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::WHOLE_INPUTS),
                                            Qt::yellow))
{
    ui->setupUi(this);
    //!
    __controller = ControllerSafety::Instance();
    connect(__controller,&ControllerSafety::initialized,this,&FrontSafetyPanel::onInitialized);
    //! Monitor
    qDebug() << __monitorTable->rowCount();
    __monitorTable->setFilter(utilities::generateFilterString(QVariant::fromValue(IoAttributes::REGION),
                                                              QVariant::fromValue(32))); //filter out safety related inputs
    ui->tableViewMonitor->setModel(__monitorTable);
    HEADER_STRUCTURE::HeaderRender::renderViewHeader(JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::HEADER_IO),
                                                     ui->tableViewMonitor);
    //! Control
    connect(ui->checkBoxSolenoid,&QCheckBox::toggled,__controller,&ControllerSafety::DoorSolenoid);
    connect(ui->checkBoxInterlockBypass,&QCheckBox::toggled,__controller,&ControllerSafety::InterlockBypass);

    //! Auth
    __disableList[AUTH::ROLE_OPERATOR].append(ui->checkBoxInterlockBypass);
}

FrontSafetyPanel::~FrontSafetyPanel()
{
    delete ui;
}

void FrontSafetyPanel::onInitialized()
{
    ui->checkBoxInterlockBypass->setChecked(__controller->Status(ControllerSafety::INTERLOCK_BYPASS));
    ui->checkBoxSolenoid->setChecked(__controller->Status(ControllerSafety::DOOR_SOLENOID_CONTROL));
}
