#ifndef TEMPCONTROLLER_H
#define TEMPCONTROLLER_H

#include <QObject>

#include <definitionbasicblocks.h>
#include <controllerbase.h>

class TempController : public ControllerBase
{
    Q_OBJECT
public:
    enum  TempProperties
    {
        PROP,
    };
    Q_ENUM(TempProperties)

    explicit TempController(QObject *parent = nullptr);

signals:

public slots:
};

#endif // TEMPCONTROLLER_H
