#ifndef MODBUSCLUSTERTEST_H
#define MODBUSCLUSTERTEST_H
#include <task.h>
#include <modbusserializedclient.h>
#include <QModbusTcpClient>
#include <QStateMachine>

class ModbusClusterTest : public Task
{
public:
    ModbusClusterTest();
protected slots:
    void onDriverConnected(QModbusDevice::State state);
    void onRequestRaised(const ModbusSegment*request);
    void onContentUpdated();
public slots:
    virtual void run(){}
protected:
    ModbusSerializedClient* client;
    ModbusCluster* cluster1;
    ModbusCluster* cluster2;
    
    QStateMachine* machine1;
    QStateMachine* machine2;
    
    quint16 memoryPool[UINT16_MAX];

    QModbusTcpClient* driver;
};

#endif // MODBUSCLUSTERTEST_H
