#include "tempcontroller.h"

TempController::TempController(QObject *parent) :
    ControllerBase(0,256,0,parent)
{
    registerWatchList(0,QVariant::fromValue(CellDataBlock()));

    qDebug() << __propertyKeys.count();
//    __propertyKeys.clear();
    __propertyKeys.append(QVariant::fromValue(PROP));
    qDebug() << __propertyKeys.count();
    qDebug() << __channel;
}
