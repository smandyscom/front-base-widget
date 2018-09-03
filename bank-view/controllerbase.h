#ifndef CONTROLLERBASE_H
#define CONTROLLERBASE_H

#include <QObject>
#include <QTimer>

#include <interfacerequest.h>
#include <interfacechannel.h>

//!
//! \brief The ControllerBase class
//! Offering base infrastructure
class ControllerBase : public QObject
{
    Q_OBJECT
public:
    explicit ControllerBase(quint8 clientId,quint16 baseOffset,int interval=100,QObject *parent = nullptr);

signals:

protected slots:
    virtual void onAcknowledged(InterfaceRequest ack);
protected:
    quint8 __clientId;
    quint16 __baseOffset;
    int __interval;

    ADDRESS_MODE toAddressMode(ADDRESS_MODE unoffseted);

    //!
    //! \brief __instance
    //! Next layer
    InterfaceChannel* __channel;
};

#endif // CONTROLLERBASE_H
