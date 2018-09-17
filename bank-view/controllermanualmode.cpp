#include "controllermanualmode.h"
#include <QDebug>
ControllerManualMode::ControllerManualMode(QObject *parent) :
    ControllerBase(0,1024,100,parent)
{
    //!
    m_monitor = new ManualModeDataBlock(registerWatchList(ManualModeDataBlock::STATUS_WORD,QVariant::fromValue(static_cast<MODBUS_U_WORD>(0))),this);
    registerWatchList(ManualModeDataBlock::MONITOR_BLOCK_HEAD,QVariant::fromValue(CellDataBlock()));
     //!
    //! \brief s1
    //!
    m_stateMachine = new QStateMachine(this);
    QState* s0 = new QState(m_stateMachine);
    QState* s1 = new QState(m_stateMachine);
    QState* s2 = new QState(m_stateMachine);
    QState* s3 = new QState(m_stateMachine);
    m_stateMap[STATE_IN_AUTO] =s0;
    m_stateMap[STATE_IDLE] = s1;
    m_stateMap[STATE_COMPLETE] = s2;
    m_stateMap[STATE_FINISH] = s3;

    //!
    //! Common
    foreach (QState* s, m_stateMap.values())
    {
        //! Report current state
        connect(s,&QState::entered,[=](){
            //! trigger read action
            m_currentState = m_stateMap.key(qobject_cast<QState*>(sender()));
        });
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
    m_stateMachine->setInitialState(s0);
    m_stateMachine->start();
    //!
    //!Monitor and Operator
    QList<QList<QVariant>> m_list=
    {
        //!
        //! \brief utilities::listupEnumVariant<ManualModeDataBlock::ManualContext>
        //! for MONITOR_DATA_HEAD , would return CellDataBlock
            utilities::listupEnumVariant<ManualModeDataBlock::ManualContext>(),
            utilities::listupEnumVariant<ManualModeDataBlock::StatusBits>(),
            utilities::listupEnumVariant<ManualModeDataBlock::ControlBits>(),
        //! Monitor post
//            utilities::listupEnumVariant<AxisMonitorBlock::OffsetMonitor>(),
//            utilities::listupEnumVariant<CylinderMonitorBlock::OffsetMonitor>(),
//            utilities::listupEnumVariant<SignalMonitor::OffsetMonitor>(),
//            utilities::listupEnumVariant<UnitMonitorBlock::OffsetMonitor>()
    };
    foreach (QList<QVariant> var, m_list)
    {
        foreach (QVariant varInner, var)
        {
            m_monitor_propertyKeys.append(var);
        }
    }
    //!Operators
    QList<QVariant> m_operator_list = {
        QVariant::fromValue(ManualModeDataBlock::BIT_1_RUN),
        QVariant::fromValue(ManualModeDataBlock::BIT_0_ENGAGED_HMI),
        QVariant::fromValue(ManualModeDataBlock::MON_CATEGRORY),
        QVariant::fromValue(ManualModeDataBlock::MON_DEVICE_INDEX),
        QVariant::fromValue(ManualModeDataBlock::COMMIT_CATEGRORY),
        QVariant::fromValue(ManualModeDataBlock::COMMIT_DEVICE_INDEX),
        QVariant::fromValue(ManualModeDataBlock::COMMIT_MODE),
        QVariant::fromValue(ManualModeDataBlock::DATA_BLOCK_HEAD),
    };
    foreach (QVariant var, m_operator_list)
    {
        m_operator_propertyKeys[var.toString()] = var;
    }
}

void ControllerManualMode::plcReady()
{
    emit operationReady();
    m_channel->Access(toAddressMode(ManualModeDataBlock::BIT_0_ENGAGED_HMI),true);
}
void ControllerManualMode::doneOn()
{
    //set RUN off
    m_channel->Access(toAddressMode(ManualModeDataBlock::BIT_1_RUN),false);
}
void ControllerManualMode::doneOff()
{

}
void ControllerManualMode::runOn()
{

}


//!
//! \brief ControllerManualMode::m_monitor_propertyKeys
//! \param key
//! \return
//! Key had been mixed , cannot seperated by ENUM head
//! Polyresolver left for front?
//QVariant ControllerManualMode::m_monitor_propertyKeys(QVariant key)
//{
//    switch (key.userType()) {
//    case value:
//        return m_monitor->Value(key);
//        break;
//    default:
//        break;
//    }

//}
void ControllerManualMode::m_operator_propertyChanged(QVariant key, QVariant value)
{
    if(key.toUInt() == ManualModeDataBlock::BIT_1_RUN && value.toBool())
    {
        //! Activated
        m_channel->Access(toAddressMode(key.toUInt()),true);
        setProperty(key.toString().toStdString().c_str(),false);
    }
    else if(key.toUInt() == ManualModeDataBlock::BIT_0_ENGAGED_HMI && !value.toBool())
    {
        //! Stop
        m_channel->Access(toAddressMode(key.toUInt()),false);
        setProperty(key.toString().toStdString().c_str(),true);
    }
    else
        ControllerBase::m_operator_propertyChanged(key,value);
}
