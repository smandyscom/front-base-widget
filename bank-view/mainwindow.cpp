#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QUrl>

#include<definitionaxisblocks.h>
#include<definitioncylinderblock.h>
#include<definitionsignalblock.h>

#include<definitioncommandblock.h>

#include<definitionunitblocks.h>

#include<QSqlRecord>

using namespace DEF_BASIC_DIMENSION;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    __isClosing = false;
    //! Initialize Modbus Serialized Client
    //! port from 10001-10008 (CH1-CH8
    QList<ModbusSerializedClient*> __list;
    for(int i=0;i<5;i++)
    {
        ModbusSerializedClient* __serializedClient = new ModbusSerializedClient(1,this);

        QModbusTcpClient* client = new QModbusTcpClient(__serializedClient);
        QUrl url = QUrl::fromUserInput(tr("169.254.28.1:%1").arg(10001+i));
        client->setConnectionParameter(QModbusDevice::NetworkAddressParameter,url.host());
        client->setConnectionParameter(QModbusDevice::NetworkPortParameter,url.port());

        __serializedClient->Driver(client);
        __list.append(__serializedClient);
    }
    //! Ch0 linked to safety controller
    QUrl url = QUrl::fromUserInput(tr("169.254.60.1:%1").arg(502));
    __list.first()->Driver()->setConnectionParameter(QModbusDevice::NetworkAddressParameter,url.host());
    __list.first()->Driver()->setConnectionParameter(QModbusDevice::NetworkPortParameter,url.port());

    ModbusChannel::Channels(__list);
    foreach (ModbusSerializedClient* arg, __list) {
        arg->Driver()->connectDevice();
    }
    //! Intialize Database
    JunctionBankDatabase::DatabaseName("base.db");
    JunctionBankDatabase::Instance()->onInitialize();
    //!
    FrontBankTransfer::ObjectTable = JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::DEF_OBJECT_TYPE);
    //! Link
    ControllerBankTransfer::Adaptor(CommitBlock::SELECTION_AXIS,new GenericSqlTableAdapter<AxisContextBlock,AxisBlock::DataBaseHeaders>(JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::WHOLE_AXIS)));
    ControllerBankTransfer::Adaptor(CommitBlock::SELECTION_CYLINDER,new CylinderSqlTableAdaptor(JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::WHOLE_CYLINDERS)));
    ControllerBankTransfer::Adaptor(CommitBlock::SELECTION_SIGNAL,new GenericSqlTableAdapter<SignalContext,SignalBlock::DataBaseHeaders>(JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::WHOLE_SIGNALS)));

    ControllerBankTransfer::Adaptor(CommitBlock::SELECTION_COMMAND_BLOCK,new GenericSqlTableAdapter<ExtendedCommandBlock,CommandBlock::DataBaseHeaders>(JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::WHOLE_COMMAND_BLOCKS)));

    ControllerBankTransfer::Adaptor(CommitBlock::SELECTION_UNIT,new GenericSqlTableAdapter<UnitContextBlock,UnitBlock::DataBaseHeaders>(JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::WHOLE_UNITS)));

    //! Basic Dimension intialization
    AbstractDataBlock::Dimension = new QMap<Keys,qreal>();
    QList<QVariant> __keys = utilities::listupEnumVariant<Keys>();
    foreach (QVariant var, __keys) {
        AbstractDataBlock::Dimension->insert(var.value<Keys>(),
                utilities::getSqlTableSelectedRecord(JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::DEF_BASIC_DIMENSION),
                                                     QVariant::fromValue(__KEY),
                                                     var).value(QVariant::fromValue(__VALUE).toString()).toReal());
    }   
    //! Material information
    QList<ControllerMaterialTransfer::SlotType> __typeList = {
        ControllerMaterialTransfer::TYPE_CREATE_NODE,//0
        ControllerMaterialTransfer::TYPE_DATA_NODE,//1
        ControllerMaterialTransfer::TYPE_EMPTY_NODE,
        ControllerMaterialTransfer::TYPE_DATA_NODE,//3
        ControllerMaterialTransfer::TYPE_DATA_NODE,//4
        ControllerMaterialTransfer::TYPE_EMPTY_NODE,
        ControllerMaterialTransfer::TYPE_EMPTY_NODE,
        ControllerMaterialTransfer::TYPE_EMPTY_NODE,
        ControllerMaterialTransfer::TYPE_DATA_NODE,//8
        ControllerMaterialTransfer::TYPE_EMPTY_NODE,
        ControllerMaterialTransfer::TYPE_EMPTY_NODE,
        ControllerMaterialTransfer::TYPE_EMPTY_NODE,
        ControllerMaterialTransfer::TYPE_EMPTY_NODE,
        ControllerMaterialTransfer::TYPE_EMPTY_NODE,
        ControllerMaterialTransfer::TYPE_EMPTY_NODE,//14
        ControllerMaterialTransfer::TYPE_DATA_NODE,//15
    };
    int counter=0;
    foreach (ControllerMaterialTransfer::SlotType var, __typeList) {
        __materialSlots.append(new ControllerMaterialTransfer(counter++,var,this));
    }
    //! Initialize FrontManaul panel
    FrontControlPanel* fcp2 = new FrontControlPanel(__materialSlots,ui->tabMain);
    FrontManaualMode* fmm = new FrontManaualMode(ui->tabManual);
    FrontIoOverride* fio = new FrontIoOverride(ui->tabIO);
    FrontCylinderPanel* fcp = new FrontCylinderPanel(ui->tabCylinder);
    FrontUnitPanel* fup = new FrontUnitPanel(ui->tabUnit);
    FrontConfigurationTransfer* fct = new FrontConfigurationTransfer(ui->tabConfiguration);
    //! Connect data changed
    connect(fmm,&FrontManaualMode::dataChanged,ControllerMainPanel::Instance(),&ControllerMainPanel::onDataChanged);
    connect(fcp,&FrontCylinderPanel::dataChanged,ControllerMainPanel::Instance(),&ControllerMainPanel::onDataChanged);
    //! Connect controller and channel
    ControllerManualMode* __controller =  ControllerManualMode::Instance();
    ModbusChannel* __channel = ModbusChannel::Instance();
    connect(__controller,&ControllerManualMode::requireReadData,__channel,[__channel](ModbusDriverAddress address,const QVariant data){
        __channel->beginAccess(address,data);
    });
    connect(__controller,&ControllerManualMode::requireWriteData,__channel,[__channel](ModbusDriverAddress address,const QVariant data){
        __channel->Access(address,data);
    });
    //!
    connect(__channel,&ModbusChannel::readReply,this,&MainWindow::onReadReply);
    //!

    //! Auth
    __authController = new ControllerAuth(this);
    __authRoleTable[ui->actionOperator] = AUTH::ROLE_OPERATOR;
    __authRoleTable[ui->actionEngineer] = AUTH::ROLE_ENGINEER;
    __authRoleTable[ui->actionDeveloper] = AUTH::ROLE_DEVELOPER;
    foreach (QObject* __action, __authRoleTable.keys()) {
        connect(qobject_cast<QAction*>(__action),
                &QAction::triggered,
                this,
                &MainWindow::onAuthTriggered);
    }
    __authController->linkAuthReceivers(this);
    connect(__authController,&ControllerAuth::onRoleChanged,this,&MainWindow::onAuthChanged);
    __authController->onAuthChangingRequired(AUTH::ROLE_OPERATOR,0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onReadReply()
{    
    ControllerManualMode::Instance()->postEvent(ModbusChannel::Instance()->EventSocket());
    foreach (ControllerMaterialTransfer* var, __materialSlots) {
        if(var->Role()==ControllerMaterialTransfer::TYPE_DATA_NODE)
            var->postEvent(ModbusChannel::Instance()->EventSocket());
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(!__isClosing)
    {
        //! inform material transfer to send DB_ENGAGED
        foreach (ControllerMaterialTransfer* var, __materialSlots) {
            var->onAboutToLeave();
        }
        __isClosing = true;
        event->ignore();
    }
    else
    {
        bool __isAllDisengaged=true;
        foreach (ControllerMaterialTransfer* var, __materialSlots) {
            if(var->ConnectionEngaged())
                __isAllDisengaged = false;
        }
        if(__isAllDisengaged)
            event->accept();
        else
            event->ignore();
    }
}

void MainWindow::onAuthTriggered()
{
    __authController->onAuthChangingRequired(__authRoleTable[sender()],
            QInputDialog::getInt(this,
                                 tr("輸入密碼"),
                                 tr("密碼")));
}
void MainWindow::onAuthChanged(QString roleName)
{
    ui->menuLogin->setTitle(tr("登入:%1").arg(roleName));
}
