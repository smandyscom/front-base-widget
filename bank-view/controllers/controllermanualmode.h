#ifndef CONTROLLERMANUALMODE_H
#define CONTROLLERMANUALMODE_H

#include <QObject>
#include <QMap>
#include <QStateMachine>
#include <QVariant>
#include <QTimer>

#include <definitionmanualblock.h>
#include <qelapsedtimer.h>

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
    

    explicit ControllerManualMode(quint8 clientId, quint16 baseOffset, int interval=100, QObject *parent=nullptr);

    //ManualModeDataBlock::ManualState CurrentState() const { return m_currentState;}

signals:
protected slots:
    void onStateReport();
protected:

    QMap<ManualModeDataBlock::ManualState,QState*> m_stateMap;
	ManualModeDataBlock::ManualState m_currentState;

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

	QElapsedTimer timer;
};

#endif // CONTROLLERMANUALMODE_H
