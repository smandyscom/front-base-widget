#include "controllermanualmode.h"
#include <QDebug>
ControllerManualMode::ControllerManualMode(QObject *parent) :
    ControllerBase(0,512,100,parent)
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
    engagedPLCOff->setTargetState(s0);
    s1->addTransition(engagedPLCOff);
    s1->addTransition(this,SIGNAL(operationTriggered()),s2);// when user triggered
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
    //!Monitor
    QList<QList<QVariant>> m_list=
    {
            utilities::listupEnumVariant<ManualModeDataBlock::ManualContext>(),
            utilities::listupEnumVariant<ManualModeDataBlock::StatusBits>(),
            utilities::listupEnumVariant<ManualModeDataBlock::ControlBits>()
    };

    foreach (QList<QVariant> var, m_list)
    {
        foreach (QVariant varInner, var)
        {
            m_monitor_propertyKeys.append(var);
        }
    }
    m_monitor_propertyKeys.append(QVariant::fromValue(ManualModeDataBlock::MONITOR_BLOCK_HEAD));
    //!Operators
    foreach (QVariant var, utilities::listupEnumVariant<ManualModeDataBlock::ControlBits>()) {
        m_operator_propertyKeys[var.toString()] = var;
    }
    m_operator_propertyKeys[QVariant::fromValue(ManualModeDataBlock::DATA_BLOCK_HEAD)] =
            QVariant::fromValue(ManualModeDataBlock::DATA_BLOCK_HEAD);
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
    switch (static_cast<ManualModeDataBlock*>(m_monitor)->Value(ManualModeDataBlock::COMMIT_MODE).value<ManualModeDataBlock::COMMIT_MODE>())
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
//!
QVariant ControllerManualMode::m_monitor_propertyKeys(QVariant key)
{

}
void ControllerManualMode::m_operator_propertyChanged(QVariant key,QVariant value)
{

}
