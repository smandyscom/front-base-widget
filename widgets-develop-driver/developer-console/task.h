#ifndef TASK_H
#define TASK_H

#include <QObject>

typedef void (*TestRoutine)();

class Task : public QObject
{
    Q_OBJECT
public:
    explicit Task(TestRoutine __routine=nullptr,QObject *parent = nullptr):
        routine(__routine),
        QObject(parent){}

signals:

public slots:
    virtual void run(){routine();}
protected:
    TestRoutine routine;
};

#endif // TASK_H
