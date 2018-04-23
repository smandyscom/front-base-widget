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
    //! Initialize Adaptors
    AxisBlock::Adaptor = new GenericSqlTableAdapter<AxisContextBlock,AxisBlock::DataBaseHeaders>(JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::WHOLE_AXIS));
    CylinderBlock::Adaptor = new CylinderSqlTableAdaptor(JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::WHOLE_CYLINDERS));
    SignalBlock::Adaptor = new GenericSqlTableAdapter<SignalContext,SignalBlock::DataBaseHeaders>(JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::WHOLE_SIGNALS));

    CommandBlock::Adaptor = new GenericSqlTableAdapter<ExtendedCommandBlock,CommandBlock::DataBaseHeaders>(JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::WHOLE_COMMAND_BLOCKS));

    UnitBlock::Adaptor = new GenericSqlTableAdapter<UnitContextBlock,UnitBlock::DataBaseHeaders>(JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::WHOLE_UNITS));

    //! Link
    ControllerBankTransfer::Instance()->Adaptor(CommitBlock::SELECTION_AXIS,AxisBlock::Adaptor);
    ControllerBankTransfer::Instance()->Adaptor(CommitBlock::SELECTION_CYLINDER,CylinderBlock::Adaptor);
    ControllerBankTransfer::Instance()->Adaptor(CommitBlock::SELECTION_SIGNAL,SignalBlock::Adaptor);

    ControllerBankTransfer::Instance()->Adaptor(CommitBlock::SELECTION_COMMAND_BLOCK,CommandBlock::Adaptor);

    ControllerBankTransfer::Instance()->Adaptor(CommitBlock::SELECTION_UNIT,CommandBlock::Adaptor);

    //! Basic Dimension intialization
    Dimension = new QMap<Keys,qreal>();
    QList<QVariant> __keys = utilities::listupEnumVariant<Keys>();
    foreach (QVariant var, __keys) {
        Dimension->insert(var.value<Keys>(),
                utilities::getSqlTableSelectedRecord(JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::DEF_BASIC_DIMENSION),
                                                     QVariant::fromValue(__KEY),
                                                     var).value(QVariant::fromValue(__VALUE).toString()).toReal());
    }
    //! Initialize FrontManaul panel
    frontControlPanel* fcp2 = new frontControlPanel(ui->tabMain);
    FrontManaualMode* fmm = new FrontManaualMode(JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::WHOLE_COMMAND_BLOCKS),
                                                 JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::WHOLE_AXIS),
                                                 JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::DEF_REGION),
                                                 ui->tabManual);
    FrontIoOverride* fio = new FrontIoOverride(JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::OUTPUT_ATTRIBUTES),
                                               JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::INPUT_ATTRIBUTES),
                                               JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::DEF_REGION),
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
