#include <QCoreApplication>
#include <QTimer>
#include <task.h>
#include <testtask2.h>
#include <timersource.h>
#include <modbuschanneltest.h>
#include <signal.h>

#include <basicblocksdefinition.h>
#include <QVariant>
#include <utilities.h>

extern void test1();
extern void test2();

TestTask2* __testTask;

void test3()
{
    //__testTask->doFake();
    __testTask->doEvent();
}

void signalHandler(int sig)
{
    while (1) {
        ;
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    signal(SIGSEGV,signalHandler);


    //
    Task* __task1 = new Task(test1,&a);
    __testTask = new TestTask2();
    timerSource* ts = new timerSource(test3,&a);
//    SerializedClientTest* serialTestTask = new SerializedClientTest();
    //ModbusClusterTest* modbusClusterTestTask  = new ModbusClusterTest();


    //QTimer::singleShot(0,__task1,SLOT(run()));
    //QTimer::singleShot(0,__testTask,SLOT(run()));
    //QTimer::singleShot(0,ts,SLOT(run()));
    //QTimer::singleShot(0,serialTestTask,SLOT(run()));
    //QTimer::singleShot(0,modbusClusterTestTask,SLOT(run()));
    ModbusChannelTest* mct = new ModbusChannelTest();

    size_t sz = sizeof(AbstractCommandBlock);//22bytes
    sz = sizeof(PosICommandBlock);//30bytes
    sz = sizeof(ZretCommandBlock);//38bytes
    AbstractCommandBlock acb = {0,BCT_NOP,0,0,0,0,0};
    PosICommandBlock pcb;


    QVariant box;
    box.setValue(acb);
    sz = sizeof(box);
    sz = utilities::sizeOf(box);

    box.setValue(pcb);
    sz = sizeof(box);
    sz = utilities::sizeOf(box);
    box = QVariant::fromValue(acb);


    return a.exec();
}
