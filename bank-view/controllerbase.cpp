#include "controllerbase.h"
#include <QDebug>

ControllerBase::ControllerBase(quint8 clientId, quint16 baseOffset, int interval, QObject *parent) :
    QObject(parent),
    __clientId(clientId),
    __baseOffset(baseOffset),
    __interval(interval)
{
    __isInitialized = false;
//    __targetWidget = static_cast<QWidget*>(parent);
    __channel = InterfaceChannel::Instance();

    __propertyKeys = QList<QVariant>();
    __watchList = QList<QVariant>();
    __propertyKeys.append(QVariant::fromValue(PROP));

    qDebug() << __propertyKeys.count();

    registerWatchList(256,QVariant::fromValue(CellDataBlock()));
    connect(__channel,&InterfaceChannel::ackownledged,this,&ControllerBase::onAcknowledged);
}

ADDRESS_MODE ControllerBase::toAddressMode(ADDRESS_MODE unoffseted) const
{
    return (unoffseted & 0x00ffffff) +
            __clientId * 0x01000000 +
            __baseOffset;
}

void ControllerBase::onAcknowledged(InterfaceRequest ack)
{
    //! could be overriden by derived
//    if (!__isInitialized)
//    {
//        onInitializing(ack);
//        __isInitialized = true;
//        return; // first time received ack
//    }

    //!if not on watch list , return (performance

    //! Raising property updating (string copy , performance issue?
    foreach (QVariant var, __propertyKeys) {
//       qDebug() << var.toString();
//       static_cast<QWidget*>(parent())->setProperty(var.toString().toStdString().c_str(),propertyValues(var));
       parent()->setProperty(var.toString().toStdString().c_str(),propertyValues(var));

    }

    //! Ui update
//    static_cast<QWidget*>(parent())->style()->unpolish(static_cast<QWidget*>(parent()));
//    static_cast<QWidget*>(parent())->style()->polish(static_cast<QWidget*>(parent()));

//    __targetWidget->style()->unpolish(__targetWidget);
//    __targetWidget->style()->polish(__targetWidget);
}



void ControllerBase::registerWatchList(ADDRESS_MODE unoffsetedAddress,QVariant form)
{
    ADDRESS_MODE __address = toAddressMode(unoffsetedAddress);
    __channel->RegisterRoutines(toAddressMode(__address),form,__interval);
}

QVariant ControllerBase::propertyValues(QVariant key)
{
    return __channel->Access<MODBUS_U_WORD>(256);
}
