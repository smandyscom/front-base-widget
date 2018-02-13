#include "modbusclustertest.h"
#include <QDebug>
#include <baselayerdefinitions.h>
#include <QUrl>


using namespace BaseLayer;

ModbusClusterTest::ModbusClusterTest()
{
    //! driver initialize
    //!
    //!
    driver = new QModbusTcpClient();
    QUrl url = QUrl::fromUserInput("127.0.0.1:502");
    driver->setConnectionParameter(QModbusTcpClient::NetworkPortParameter,url.port());
    driver->setConnectionParameter(QModbusTcpClient::NetworkAddressParameter,url.host());
    driver->setTimeout(1000);
    driver->setNumberOfRetries(3);
    bool result = driver->connectDevice();
    qDebug() << QString("Driver connection:%1").arg(result);
    qDebug() << QString("Driver state:%1").arg(QVariant::fromValue(QModbusDevice::State(driver->state())).value<QString>());
    //! connected
    connect(driver,&QModbusTcpClient::stateChanged,this,&ModbusClusterTest::onDriverConnected);

    //! Cluster initialize
    //!
    ModbusCluster::Attributes attr;
    attr.setFlag(ModbusCluster::READ);
    attr.setFlag(ModbusCluster::WRITE);

    cluster1 = new ModbusCluster(&memoryPool[0]
            ,4
            ,0
            ,attr);
    cluster2 = new ModbusCluster(&memoryPool[4]
            ,4
            ,0
            ,attr);

    connect(cluster1,&ModbusCluster::sendModbusRequest,this,&ModbusClusterTest::onRequestRaised);
    connect(cluster2,&ModbusCluster::sendModbusRequest,this,&ModbusClusterTest::onRequestRaised);

    connect(cluster1,&ModbusCluster::clusterUpdated,this,&ModbusClusterTest::onContentUpdated);
    connect(cluster2,&ModbusCluster::clusterUpdated,this,&ModbusClusterTest::onContentUpdated);


    //! State machine preparation (highest stack
    //!
    //!
    machine1 = new QStateMachine(this);
    QState* s1 = new QState(machine1);
    //connect(s1,&QState::entered,this,[this](){cluster1->beginUpdateCluster();});

    connect(s1,&QState::exited,this,[this](){
        qDebug()
                << QString("Machine 1 read %1\n").arg(memoryPool[0],0,16)
                << QString("Machine 1 read %1\n").arg(memoryPool[1],0,16)
                << QString("Machine 1 read %1\n").arg(memoryPool[2],0,16)
                << QString("Machine 1 read %1\n").arg(memoryPool[3],0,16);
    });

    ValueTransition* vt1 = new ValueTransition(ModbusDriverAddress(0x00000000),ValueTransition::VALUE_UPDATED);
    vt1->setTargetState(s1);
    s1->addTransition(vt1);

    machine1->setInitialState(s1);


    //machine2 = new QStateMachine(this);
}

void ModbusClusterTest::onDriverConnected(QModbusDevice::State state)
{
    qDebug() << QString("Driver state:%1").arg(QVariant::fromValue(state).value<QString>());
    if(driver->state() != QModbusDevice::ConnectedState)
        return;

    connect(driver->sendReadRequest(QModbusDataUnit(QModbusDataUnit::HoldingRegisters,1,1),1),
            &QModbusReply::finished,
            this,
            [](){qDebug()<<"Read done";});

    //! client initialize
    client = new ModbusSerializedClient(driver);

    //!
    //! activate state machine
    machine1->start();
    cluster1->beginUpdateCluster();//keep polling
}

void ModbusClusterTest::onRequestRaised(const ModbusSegment * request)
{
    client->pushRequest(request);
}

void ModbusClusterTest::onContentUpdated()
{
    //scan all interested address , once it is in updated cluster , raise event
    machine1->postEvent(new UpdateEvent(ModbusDriverAddress(0x00000000),QVariant::fromValue(memoryPool[0])));
    cluster1->beginUpdateCluster();//keep polling
}
