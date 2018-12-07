#ifndef CONTROLLERBASE_H
#define CONTROLLERBASE_H

#include <QObject>
#include <QTimer>
#include <QMap>
#include <QList>
#include <QDebug>
#include <QVariant>

#include <interfacerequest.h>
#include <interfacechannel.h>

#include <definitionbasicblocks.h>


#include <propertyportcommon.h>
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

    

	QObject* port() const;

    //bool event(QEvent *event) Q_DECL_OVERRIDE;
protected slots:
    //!
    //! \brief onAcknowledged
    //! \param ack
    //! Routine
    virtual void onAcknowledged(InterfaceRequest ack);
    virtual void onPropertyChanged(QVariant key,QVariant value);

	virtual void onUpdate();
protected:
	virtual void attach(QObject* receiver);

    quint8 m_clientId;
    quint16 m_baseOffset;
    int m_interval;

    InterfaceChannel* m_channel;

    MODBUS_U_WORD *registerWatchList(ADDRESS_MODE unoffsetedAddress,QVariant form,QStateMachine* machine=nullptr,bool isMachineWatchOnly=false);
    ADDRESS_MODE toAddressMode(ADDRESS_MODE unoffseted) const;

    //! Monitor
    AbstractDataBlock* m_monitor;
    QList<QVariant> m_monitor_propertyKeys;
    virtual QVariant m_monitor_propertyValues(QVariant key);
    //!
    PropertyPortCommon* m_port;


	//
	QTimer* m_updateTimer;
};

#endif // CONTROLLERBASE_H
