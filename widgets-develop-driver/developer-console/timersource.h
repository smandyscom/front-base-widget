#ifndef TIMERSOURCE_H
#define TIMERSOURCE_H

#include <QObject>
#include <QTimer>
#include <task.h>
class timerSource : public Task
{
    Q_OBJECT
public:
    explicit timerSource(TestRoutine routine,QObject *parent = nullptr);
    QTimer* timer;
signals:

public slots:
    virtual void run();
    void updated(){routine();}
};

#endif // TIMERSOURCE_H
