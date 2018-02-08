#include <QCoreApplication>
#include <QTimer>
#include <task.h>
#include <testtask2.h>
#include <timersource.h>
extern void test1();
extern void test2();

TestTask2* __testTask;

void test3()
{
    //__testTask->doFake();
    __testTask->doEvent();
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);



    //
    Task* __task1 = new Task(test1,&a);
    __testTask = new TestTask2();
    timerSource* ts = new timerSource(test3,&a);

    QTimer::singleShot(0,__task1,SLOT(run()));
    QTimer::singleShot(0,__testTask,SLOT(run()));
    QTimer::singleShot(0,ts,SLOT(run()));

    return a.exec();
}
