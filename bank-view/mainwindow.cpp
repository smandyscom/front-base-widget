#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QUrl>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);



    //! Initialize Modbus Serialized Client
    //! port from 10001-10008 (CH1-CH8
    QList<ModbusSerializedClient*> __list;
    for(int i=0;i<4;i++)
    {
        QModbusTcpClient* client = new QModbusTcpClient(this);
        QUrl url = QUrl::fromUserInput(tr("169.254.28.1:%1").arg(10001+i));
        //QUrl url = QUrl::fromUserInput(tr("127.0.0.1:%1").arg(500+1+i));
        client->setConnectionParameter(QModbusDevice::NetworkAddressParameter,url.host());
        client->setConnectionParameter(QModbusDevice::NetworkPortParameter,url.port());
        ModbusSerializedClient* __serializedClient = new ModbusSerializedClient(client);
        __list.append(__serializedClient);
    }
    ModbusChannel::Channels(__list);
    foreach (ModbusSerializedClient* arg, __list) {
        arg->Driver()->connectDevice();
    }


    //! Intialize Database
    JunctionBankDatabase::DatabaseName("base.db");
    JunctionBankDatabase::Instance()->onInitialize();

    //! Initialize FrontManaul panel
    FrontManaualMode* fmm = new FrontManaualMode(JunctionBankDatabase::Instance()->CommandBlockTable(),
                                                 JunctionBankDatabase::Instance()->AxisTable(),
                                                 JunctionBankDatabase::Instance()->RegionTable(),
                                                 ui->tabManual);
    FrontIoOverride* fio = new FrontIoOverride(JunctionBankDatabase::Instance()->InputTable(),
                                               JunctionBankDatabase::Instance()->OutputTable(),
                                               JunctionBankDatabase::Instance()->RegionTable(),
                                               ui->tabIO);
    FrontCylinderPanel* fcp = new FrontCylinderPanel(ui->tabCylinder);

    //! Connect controller and channel
    ControllerManualMode* __controller =  ControllerManualMode::Instance();
    ModbusChannel* __channel = ModbusChannel::Instance();
    connect(__controller,&ControllerManualMode::requireReadData,__channel,[__channel](ModbusDriverAddress address,const QVariant data){
        __channel->beginAccess(address,data);
    });
    connect(__controller,&ControllerManualMode::requireWriteData,__channel,[__channel](ModbusDriverAddress address,const QVariant data){
        __channel->Access(address,data);
    });
    connect(__channel,SIGNAL(raiseUpdateEvent(UpdateEvent*)),this,SLOT(onRaiseUpdateEvent(UpdateEvent*)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onRaiseUpdateEvent(UpdateEvent *e)
{
    ControllerManualMode::Instance()->postEvent(e);
}
