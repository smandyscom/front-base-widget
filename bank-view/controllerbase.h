#ifndef CONTROLLERBASE_H
#define CONTROLLERBASE_H

#include <QObject>
#include <QTimer>
#include <QMap>
#include <QList>
#include <QWidget>
#include <QDebug>

#include <interfacerequest.h>
#include <interfacechannel.h>

#include <definitionbasicblocks.h>

//!
//! \brief The ControllerBase class
//! Offering base infrastructure
//! Parent as Widget UI
//! Turn low-level info into Properties (Key-Value pair) , to update UI
class ControllerBase : public QObject
{
    Q_OBJECT

//using VALUE_FETCHER = QVariant (ControllerBase::*)();

public:
    enum Temp
    {
      PROP,
    };
    Q_ENUM(Temp)

    explicit ControllerBase(quint8 clientId,quint16 baseOffset,int interval,QObject *parent = nullptr);

protected slots:
    //!
    //! \brief onAcknowledged
    //! \param ack
    //! Routine
    virtual void onAcknowledged(InterfaceRequest ack);
    //!
    //! \brief onInitializing
    //! First time received ack
    virtual void onInitializing(InterfaceRequest ack){}
protected:
    quint8 __clientId;
    quint16 __baseOffset;
    int __interval;
    bool __isInitialized;
    InterfaceChannel* __channel;

    QList<QVariant> __propertyKeys;
    QList<QVariant> __watchList;

    void *registerWatchList(ADDRESS_MODE unoffsetedAddress,QVariant form);
    ADDRESS_MODE toAddressMode(ADDRESS_MODE unoffseted) const;

    //!
    //! \brief __instance
    //! Next layer


    virtual QVariant propertyValues(QVariant key);

private:


};

#endif // CONTROLLERBASE_H
