#ifndef CONTROLLERBASE_H
#define CONTROLLERBASE_H

#include <QObject>
#include <QTimer>
#include <QMap>
#include <QList>
#include <QWidget>
#include <QDebug>
#include <QVariant>

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

public:
    explicit ControllerBase(quint8 clientId,quint16 baseOffset,int interval,QObject *parent = nullptr);

    bool event(QEvent *event) Q_DECL_OVERRIDE;
protected slots:
    //!
    //! \brief onAcknowledged
    //! \param ack
    //! Routine
    virtual void onAcknowledged(InterfaceRequest ack);
    //!
    //! \brief onInitializing
    //! First time received ack
    virtual void onInitializing(InterfaceRequest ack){ m_isInitialized = true; }
protected:
    quint8 m_clientId;
    quint16 m_baseOffset;
    int m_interval;
    bool m_isInitialized;
    InterfaceChannel* m_channel;

    MODBUS_U_WORD *registerWatchList(ADDRESS_MODE unoffsetedAddress,QVariant form);
    ADDRESS_MODE toAddressMode(ADDRESS_MODE unoffseted) const;

    //! Monitor
    AbstractDataBlock* m_monitor;
    QList<QVariant> m_monitor_propertyKeys;
    virtual QVariant m_monitor_propertyValues(QVariant key);
    //! Operator
    QMap<QString,QVariant> m_operator_propertyKeys;
    virtual void m_operator_propertyChanged(QVariant key,QVariant value){}

};

#endif // CONTROLLERBASE_H
