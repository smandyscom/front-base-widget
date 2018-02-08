#include "testtask2.h"
#include <baselayerdefinitions.h>
#include <QDebug>
#include <QStateMachine>
using namespace BaseLayer;

TestTask2::TestTask2():
    Task(nullptr,nullptr)
{

}

void TestTask2::run()
{
     __machine = new QStateMachine(nullptr);
    QState* s1 = new QState(__machine);
    QState* s2 = new QState(__machine);

    connect(s1,&QState::entered,this,[]{qDebug() << "s1";});
    connect(s2,&QState::entered,this,[]{qDebug() << "s2";});

    ModbusDriverAddress* mda = new ModbusDriverAddress(0x00150000); // bit index 5, 0x20
    ValueTransition* vt = new ValueTransition(mda,ValueTransition::BIT_STATE_ON);
    ValueTransition* vt2 = new ValueTransition(mda,ValueTransition::BIT_STATE_ON);

    vt->setTargetState(s2);
    vt2->setTargetState(s1);

    s1->addTransition(vt);
    s1->addTransition(this,SIGNAL(fake()),s2);

    s2->addTransition(vt2);

    __machine->setInitialState(s1);
    __machine->start();

//    __machine->postEvent(new UpdateEvent(mda,QVariant(0x00000010)));
//     __machine->postEvent(new UpdateEvent(mda,QVariant(0x00000020)));
//      __machine->postEvent(new UpdateEvent(mda,QVariant(0x00000040)));

//      emit fake();
}


