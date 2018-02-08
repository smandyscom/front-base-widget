#include "timersource.h"

timerSource::timerSource(TestRoutine routine,QObject *parent) :
    Task(routine,parent)
{

}

void timerSource::run()
{
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(updated()));
    timer->start(1000);
}
