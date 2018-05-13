#include "controllermanualmode.h"
#include <QDebug>
ControllerManualMode::ControllerManualMode(QObject *parent) :
    QStateMachine(parent)
{
    //! Channel initialize
    __channel = ModbusChannel::Instance();
    CommitOption(CommitBlock());

    //! Very first shot
    __channel->beginAccess<AbstractDataBlock>(ModbusDriverAddress(MONITOR_BLOCK_HEAD));
    __channel->beginAccess<MODBUS_U_WORD>(ModbusDriverAddress(STATUS_WORD));
    __channel->beginAccess<IoMonitorOverrideBlock>(ModbusDriverAddress(IO_MON_OVERRIDE));
    //!
    //! \brief s1
    //!
    QState* s0 = new QState(this);
    QState* s1 = new QState(this);
    QState* s2 = new QState(this);
    QState* s3 = new QState(this);
    __stateMap[STATE_IN_AUTO] =s0;
    __stateMap[STATE_IDLE] = s1;
    __stateMap[STATE_COMPLETE] = s2;
    __stateMap[STATE_FINISH] = s3;

    //!
    //! Common
    foreach (QState* s, __stateMap.values())
    {
        //! Report current state
        connect(s,&QState::entered,this,[this](){
            //! trigger read action
            __currentState = __stateMap.key(qobject_cast<QState*>(sender()));
        });
    }

    //!
    //! s0
    ValueTransition* engagedPLCOn = new ValueTransition(ModbusDriverAddress(ENGAGED_SEMI_AUTO),ValueTransition::BIT_STATE_ON);
    engagedPLCOn->setTargetState(s1);
    s0->addTransition(engagedPLCOn);

    //!
    //! s1
    ValueTransition* engagedPLCOff = new ValueTransition(ModbusDriverAddress(ENGAGED_SEMI_AUTO),ValueTransition::BIT_STATE_OFF);
    engagedPLCOff->setTargetState(s0);
    s1->addTransition(engagedPLCOff);
    s1->addTransition(this,SIGNAL(operationTriggered()),s2);// when user triggered

    connect(s1,&QState::exited,[this](){
        //! commit block if need
        //! Exit by triggered
        if(__channel->Access<bool>(ModbusDriverAddress(ENGAGED_SEMI_AUTO))){
            emit requireWriteData(ModbusDriverAddress(ENGAGED_HMI),QVariant::fromValue(true));
            emit requireWriteData(ModbusDriverAddress(RUN),QVariant::fromValue(true));//set Run on
        }
    });
    //!
    //! s2
    ValueTransition* doneOn = new ValueTransition(ModbusDriverAddress(DONE),ValueTransition::BIT_STATE_ON);
    doneOn->setTargetState(s3);

    s2->addTransition(doneOn); //when DONE on
    connect(s2,&QState::exited,[this](){

        //!
        //! read out block if need
        switch (CommitOption().Mode()) {
        case CommitBlock::MODE_UPLOAD_DATA_BLOCK:
            //! should read full size
            emit requireReadData(ModbusDriverAddress(DATA_BLOCK_HEAD),QVariant::fromValue(AbstractDataBlock()));
            break;
        default:
            break;
        }

        //set RUN off
        emit requireWriteData(ModbusDriverAddress(RUN),QVariant::fromValue(false));
    });
    //!
    //! s3
    ValueTransition* doneOff = new ValueTransition(ModbusDriverAddress(DONE),ValueTransition::BIT_STATE_OFF);

    doneOff->setTargetState(s0);

    s3->addTransition(doneOff); //when DONE off
    connect(s3,&QState::exited,[this](){
           emit operationPerformed();//inform required operation had performed
    });
    //!
    connect(__channel,SIGNAL(raiseUpdateEvent(UpdateEvent*)),this,SLOT(onReply(UpdateEvent*)));

    //!
    setInitialState(s0);
    start();
}

//!
//! \brief ControllerManualMode::onMonitorBlockReply
//! \param event
//! intercept monitor block updating event
void ControllerManualMode::onReply(UpdateEvent *event)
{
    switch (event->address) {
    case MONITOR_BLOCK_HEAD:
    {
        //! keep polling monitor status
        QTimer::singleShot(100,this,[this](){
            //Schedual the next polling
            __channel->beginAccess<AbstractDataBlock>(ModbusDriverAddress(MONITOR_BLOCK_HEAD));
        });
        break;
    }
    case STATUS_WORD:
    {
        QTimer::singleShot(5,this,[this](){
            //Schedual the next polling
            __channel->beginAccess<MODBUS_U_WORD>(ModbusDriverAddress(STATUS_WORD));
        });
        break;
    }
    case IO_MON_OVERRIDE:
    {
        QTimer::singleShot(100,this,[this](){
            //Schedual the next polling
            //polling 8 Words so far
            __channel->beginAccess<IoMonitorOverrideBlock>(ModbusDriverAddress(IO_MON_OVERRIDE));
        });
        break;
    }
    default:
        break;
    }
}

//!
//! \brief Instance
//! \return
//!
ControllerManualMode* ControllerManualMode::Instance()
{
    if(__instance == nullptr)
        __instance = new ControllerManualMode();
    return __instance;
}

ControllerManualMode* ControllerManualMode::__instance = nullptr;
