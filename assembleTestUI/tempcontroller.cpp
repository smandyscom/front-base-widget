#include "tempcontroller.h"

TempController::TempController(QObject *parent) :
    ControllerBase(0,256,0,parent)
{
    registerWatchList(0,QVariant::fromValue(CellDataBlock()));

    qDebug() << m_monitor_propertyKeys.count();
//    __propertyKeys.clear();
    m_monitor_propertyKeys.append(QVariant::fromValue(PROP));
    qDebug() << m_monitor_propertyKeys.count();
    qDebug() << m_channel;
}
