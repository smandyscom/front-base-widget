#ifndef CONTROLLERMANUALMODE_H
#define CONTROLLERMANUALMODE_H

#include <QObject>
#include <QMap>
#include <QStateMachine>
#include <QVariant>
#include <QTimer>

#include <definitionmanualblock.h>

using namespace BaseLayer;

#include <controllerbase.h>


#include <utilities.h>

//!
//! \brief The ControllerManualMode class
//! The state machine about to control manual mode
//! How pass dynamic property to multiple parent
//!
class ControllerManualMode :
        public ControllerBase
{
    Q_OBJECT
public:
    enum ManualState : int
    {
        STATE_IN_AUTO,
        STATE_PLC_READY,
        STATE_RUN_ON,
        STATE_DONE_ON,
    };
    Q_ENUM(ManualState)

    explicit ControllerManualMode(quint8 clientId, quint16 baseOffset, int interval=100, QObject *parent=nullptr);

    ManualState CurrentState() const { return m_currentState;}

signals:
    //!
    //! \brief triggerOperation
    //! Linked to S1 transition condition
    void operationTriggered();
    void operationReady();
protected slots:
    void onStateReport();
protected:

    QMap<ManualState,QState*> m_stateMap;
    ManualState m_currentState;

    QStateMachine* m_stateMachine;
    //!
    //! \brief s1Entered
    //! PLC ready
    virtual void plcReady();
    //!
    //! \brief s2Existed
    //! Run on
    virtual void runOn();
    //!
    //! \brief s2Exited
    //! Done on
    virtual void doneOn();
    //!
    //! \brief s3Exited
    //! Done off
    virtual void doneOff();
    //!
    //! \brief m_monitor_propertyValues
    //! \param key
    //! \return
    //! Overrides
//    QVariant m_monitor_propertyValues(QVariant key) Q_DECL_OVERRIDE;
    void m_operator_propertyChanged(QVariant key,QVariant value) Q_DECL_OVERRIDE;
};

#endif // CONTROLLERMANUALMODE_H
