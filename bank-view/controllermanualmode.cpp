#include "controllermanualmode.h"
#include <QDebug>
ControllerManualMode::ControllerManualMode(QObject *parent) :
    ControllerBase(0,1024,100,parent)
{
    //!
    m_monitor = new ManualModeDataBlock(registerWatchList(ManualModeDataBlock::STATUS_WORD,QVariant::fromValue(static_cast<MODBUS_U_WORD>(0))),this);
    registerWatchList(ManualModeDataBlock::MONITOR_BLOCK_HEAD,QVariant::fromValue(CellDataBlock()));
    //! Initiate Monitor Data Block
//    MODBUS_U_WORD* m_monitor_anchor = m_channel->Handle(toAddressMode(ManualModeDataBlock::MONITOR_BLOCK_HEAD));
//    m_monitor_categrories[ManualModeDataBlock::SELECTION_AXIS] = new AxisMonitorBlock(m_monitor_anchor,this);
//    m_monitor_categrories[ManualModeDataBlock::SELECTION_CYLINDER] = new CylinderMonitorBlock(m_monitor_anchor,this);
//    m_monitor_categrories[ManualModeDataBlock::SELECTION_SIGNAL] = new SignalMonitor(m_monitor_anchor,this);
//    m_monitor_categrories[ManualModeDataBlock::SELECTION_UNIT] = new UnitMonitorBlock(m_monitor_anchor,this);
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
    engagedPLCOff->setTargetState(s0);
    s1->addTransition(engagedPLCOff);   //when manual mode offline
    s1->addTransition(this,SIGNAL(operationTriggered()),s2);// when user triggered , TODO , triggered by RUN bit on?
    connect(s1,&QState::exited,this,&ControllerManualMode::s1Exited);
    //!
    //! s2
    ValueTransition* doneOn = new ValueTransition(toAddressMode(ManualModeDataBlock::BIT_1_DONE),ValueTransition::BIT_STATE_ON);
    doneOn->setTargetState(s3);
    s2->addTransition(doneOn); //when DONE on
    connect(s2,&QState::exited,this,&ControllerManualMode::s2Exited);
    //!
    //! s3
    ValueTransition* doneOff = new ValueTransition(toAddressMode(ManualModeDataBlock::BIT_1_DONE),ValueTransition::BIT_STATE_OFF);
    doneOff->setTargetState(s0);
    s3->addTransition(doneOff); //when DONE off
    connect(s3,&QState::exited,this,&ControllerManualMode::s3Exited);
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
    QList<ManualModeDataBlock::ManualContext> m_operator_list = {
        ManualModeDataBlock::MON_CATEGRORY,
        ManualModeDataBlock::MON_DEVICE_INDEX,
        ManualModeDataBlock::COMMIT_CATEGRORY,
        ManualModeDataBlock::COMMIT_DEVICE_INDEX,
        ManualModeDataBlock::COMMIT_MODE,
        ManualModeDataBlock::DATA_BLOCK_HEAD,
    };
    foreach (ManualModeDataBlock::ManualContext var, m_operator_list)
    {
        auto qvar = QVariant::fromValue(var);
        m_operator_propertyKeys[qvar.toString()] = qvar;
    }
}

void ControllerManualMode::s1Exited()
{
    //! commit block if need
    //! Exit by triggered
    if(static_cast<ManualModeDataBlock*>(m_monitor)->Value(ManualModeDataBlock::BIT_0_ENGAGED_SEMI_AUTO).toBool())
    {
        m_channel->Access(toAddressMode(ManualModeDataBlock::BIT_0_ENGAGED_HMI),true);
        m_channel->Access(toAddressMode(ManualModeDataBlock::BIT_1_RUN),true);
    }
}
void ControllerManualMode::s2Exited()
{
    //!
    //! read out block if need
    switch (static_cast<ManualModeDataBlock*>(m_monitor)->Value(ManualModeDataBlock::COMMIT_MODE).value<ManualModeDataBlock::CommitMode>())
    {
    case ManualModeDataBlock::MODE_UPLOAD_DATA_BLOCK:
        //! should read full size
        m_channel->Access(toAddressMode(ManualModeDataBlock::DATA_BLOCK_HEAD),QVariant::fromValue(CellDataBlock()));
        break;
    default:
        break;
    }

    //set RUN off
    m_channel->Access(toAddressMode(ManualModeDataBlock::BIT_1_RUN),false);
}
void ControllerManualMode::s3Exited()
{
    emit operationPerformed();//inform required operation had performed
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

