#ifndef TESTTASK2_H
#define TESTTASK2_H
#include <task.h>
#include <QObject>
#include <QStateMachine>
#include <baselayerdefinitions.h>
using namespace BaseLayer;


class TestTask2 : public Task
{
    Q_OBJECT
public:
    TestTask2();

    void doFake(){emit fake();}
    void doEvent(){__machine->postEvent(new UpdateEvent(ModbusDriverAddress(0x00150000),QVariant(0x00000020)));}
signals:
    void fake();
public slots:
    virtual void run();

protected:
    QStateMachine* __machine;
};

#endif // TESTTASK2_H
