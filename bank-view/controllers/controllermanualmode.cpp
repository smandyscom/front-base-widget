#include "controllermanualmode.h"
#include <QDebug>


ControllerManualMode::ControllerManualMode(quint8 clientId,
                                           quint16 baseOffset,
                                           int interval,
                                           QObject *parent) :
    ControllerBase(clientId,baseOffset,interval,parent)
{
	//!
	m_stateMachine = new QStateMachine(this);
    m_monitor = new ManualModeDataBlock(registerWatchList(ManualModeDataBlock::STATUS_WORD,QVariant::fromValue(static_cast<MODBUS_U_WORD>(0)), m_stateMachine),this);
	registerWatchList(ManualModeDataBlock::CONTROL_WORD, QVariant::fromValue(static_cast<MODBUS_U_WORD>(0)), m_stateMachine,true);
    registerWatchList(ManualModeDataBlock::MONITOR_BLOCK_HEAD,QVariant::fromValue(CellDataBlock()));

     //!
    //! \brief s1
    //!
    QState* s0 = new QState(m_stateMachine);
    QState* s1 = new QState(m_stateMachine);
    QState* s2 = new QState(m_stateMachine);
    QState* s3 = new QState(m_stateMachine);
    m_stateMap[ManualModeDataBlock::STATE_IN_AUTO] =s0;
    m_stateMap[ManualModeDataBlock::STATE_PLC_READY] = s1;
    m_stateMap[ManualModeDataBlock::STATE_RUN_ON] = s2;
    m_stateMap[ManualModeDataBlock::STATE_DONE_ON] = s3;
    //!
    //! Common
    foreach (QState* s, m_stateMap.values())
    {
        //! Report current state
        connect(s,&QState::entered,this,&ControllerManualMode::onStateReport);
    }

    //!
    //! s0
    ValueTransition* engagedPLCOn = new ValueTransition(toAddressMode(ManualModeDataBlock::BIT_0_ENGAGED_SEMI_AUTO),ValueTransition::BIT_STATE_ON);
    engagedPLCOn->setTargetState(s1);
    s0->addTransition(engagedPLCOn);
    //!
    //! s1
    ValueTransition* engagedPLCOff = new ValueTransition(toAddressMode(ManualModeDataBlock::BIT_0_ENGAGED_SEMI_AUTO),ValueTransition::BIT_STATE_OFF);
    ValueTransition* runOn = new ValueTransition(toAddressMode(ManualModeDataBlock::BIT_1_RUN),ValueTransition::BIT_STATE_ON);
    engagedPLCOff->setTargetState(s0);
    runOn->setTargetState(s2);
    s1->addTransition(engagedPLCOff);   //when manual mode offline
    s1->addTransition(runOn);// when user triggered , TODO , triggered by RUN bit on?
    connect(s1,&QState::entered,this,&ControllerManualMode::plcReady);
    //!
    //! s2
    ValueTransition* doneOn = new ValueTransition(toAddressMode(ManualModeDataBlock::BIT_1_DONE),ValueTransition::BIT_STATE_ON);
    doneOn->setTargetState(s3);
    s2->addTransition(doneOn); //when DONE on
    connect(s2,&QState::entered,this,&ControllerManualMode::runOn);
    connect(s2,&QState::exited,this,&ControllerManualMode::doneOn);
    //!
    //! s3
    ValueTransition* doneOff = new ValueTransition(toAddressMode(ManualModeDataBlock::BIT_1_DONE),ValueTransition::BIT_STATE_OFF);
    doneOff->setTargetState(s0);
    s3->addTransition(doneOff); //when DONE off
    connect(s3,&QState::exited,this,&ControllerManualMode::doneOff);
    
    //!
    //!Monitor and Operator
    QList<QList<QVariant>> m_list=
    {
            utilities::listupEnumVariant<ManualModeDataBlock::ManualContext>(),
            utilities::listupEnumVariant<ManualModeDataBlock::StatusBits>(),
            utilities::listupEnumVariant<ManualModeDataBlock::ControlBits>(),
    };
    foreach (QList<QVariant> var, m_list)
    {
        foreach (QVariant varInner, var)
        {
            m_monitor_propertyKeys.append(var);
        }
    }
	//!
	m_stateMachine->setInitialState(s0);
}

void ControllerManualMode::onStateMachineStart()
{
	m_stateMachine->start();
}

void ControllerManualMode::onStateReport()
{
    //! trigger read action
    m_currentState = m_stateMap.key(qobject_cast<QState*>(sender()));
	
	emit m_port->externalPropertyChange(QVariant::fromValue(ManualModeDataBlock::PROP_MANUAL_STATE), QVariant::fromValue(m_currentState));

    qDebug() << QVariant::fromValue(m_currentState).toString();
}

void ControllerManualMode::plcReady()
{
	//reset
	onPropertyChanged(QVariant::fromValue(ManualModeDataBlock::BIT_0_ENGAGED_HMI), false);
	onPropertyChanged(QVariant::fromValue(ManualModeDataBlock::BIT_1_RUN), false);
}
void ControllerManualMode::doneOn()
{
	//ms
	quint64 e = timer.elapsed();
	emit m_port->externalPropertyChange(QVariant::fromValue(ManualModeDataBlock::PROP_ELAPSED_TIME), e);

	onPropertyChanged(QVariant::fromValue(ManualModeDataBlock::BIT_1_RUN), false);
	
}
void ControllerManualMode::doneOff()
{

}
void ControllerManualMode::runOn()
{
	//set
	onPropertyChanged(QVariant::fromValue(ManualModeDataBlock::BIT_0_ENGAGED_HMI), true);

	timer.start();
}

void ControllerManualMode::onPropertyChanged(QVariant key, QVariant value)
{
	QVariant sb = QVariant::fromValue(ManualModeDataBlock::BIT_1_RUN);
	ADDRESS_MODE address = key.toUInt();
	address = ADDRESS_REGISTER(address);
	
	if (address == ManualModeDataBlock::CONTROL_WORD &&
		!(key.userType()==sb.userType()))
	{
		return; // do not perform further more
	}
	//!
	ControllerBase::onPropertyChanged(key, value);
}