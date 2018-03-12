#include "controllermanualmode.h"
#include <QDebug>
ControllerManualMode::ControllerManualMode(QObject *parent) :
    QStateMachine(parent)
{
    //! Channel initialize
    __channel = ModbusChannel::Instance();
    __commitOption.Mode(CommitBlock::MODE_COMMAND_BLOCK);

    //! Very first shot
    __channel->beginUpdate(ModbusDriverAddress(POS_COMMAND),QVariant::fromValue(__monitorBlock));
    __channel->beginUpdate(ModbusDriverAddress(STATUS_WORD),QVariant::fromValue(static_cast<MODBUS_WORD>(0)));
    //!
    //! \brief s1
    //!
    QState* s1 = new QState(this);
    QState* s2 = new QState(this);
    QState* s3 = new QState(this);

    //!
    //! \brief engagedPLCOff
    //! Common transition
    ValueTransition* engagedPLCOff = new ValueTransition(ModbusDriverAddress(ENGAGED_PLC),ValueTransition::BIT_STATE_OFF);
    engagedPLCOff->setTargetState(s1);
    QList<QState*> states({s1,s2,s3});
    foreach (QState* s, states)
    {
        //!
        //! Link common transition
        //!
        s->addTransition(engagedPLCOff);

        connect(s,&QState::entered,this,[this](){
            //! trigger read action
            __currentState = qobject_cast<QState*>(sender());
        });
    }


    //!
    //! s1
    s1->addTransition(this,SIGNAL(operationTriggered()),s2);// when user triggered
    connect(s1,&QState::exited,[this](){
        //!
        //! commit block if need
        switch (__commitOption.Mode()) {
        case CommitBlock::MODE_DOWNLOAD:
        case CommitBlock::MODE_COMMAND_BLOCK:
            //! Always write-in full-size
            emit requireWriteData(ModbusDriverAddress(DATA_BLOCK_HEAD),QVariant::fromValue(__commandBlock));
            break;
        default:
            break;
        }

        emit requireWriteData(ModbusDriverAddress(COMMIT_BLOCK),QVariant::fromValue(__commitOption));
        emit requireWriteData(ModbusDriverAddress(ENGAGED_HMI),QVariant::fromValue(true));
        emit requireWriteData(ModbusDriverAddress(RUN),QVariant::fromValue(true));//set Run on
    });
    //!
    //! s2
    ValueTransition* doneOn = new ValueTransition(ModbusDriverAddress(DONE),ValueTransition::BIT_STATE_ON);
    doneOn->setTargetState(s3);

    s2->addTransition(doneOn); //when DONE on
    connect(s2,&QState::entered,[this](){
       qDebug() << "S2";
    });
    connect(s2,&QState::exited,[this](){

        //!
        //! read out block if need
        switch (__commitOption.Mode()) {
        case CommitBlock::MODE_UPLOAD:
            //! should read full size
            emit requireReadData(ModbusDriverAddress(DATA_BLOCK_HEAD),QVariant::fromValue(__commandBlock));
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

    doneOff->setTargetState(s1);

    s3->addTransition(doneOff); //when DONE off
    connect(s3,&QState::exited,[this](){
           emit operationPerformed();//inform required operation had performed
    });
    //!
    connect(__channel,SIGNAL(raiseUpdateEvent(UpdateEvent*)),this,SLOT(onReply(UpdateEvent*)));

    //!
    setInitialState(s1);
    start();
}

//!
//! \brief ControllerManualMode::onMonitorBlockReply
//! \param event
//! intercept monitor block updating event
void ControllerManualMode::onReply(UpdateEvent *event)
{
    switch (event->address) {
    case POS_COMMAND:
    {
        //! keep polling monitor status
        QVariant value = QVariant::fromValue(__monitorBlock);
        __channel->update(ModbusDriverAddress(POS_COMMAND),value);
        __monitorBlock = value.value<AbstractMonitorBlock>();
        QTimer::singleShot(100,this,[this](){
            //Schedual the next polling
            __channel->beginUpdate(ModbusDriverAddress(POS_COMMAND),QVariant::fromValue(__monitorBlock));
        });
        break;
    }
    case STATUS_WORD:
        QTimer::singleShot(100,this,[this](){
           //Schedual the next polling
            __channel->beginUpdate(ModbusDriverAddress(STATUS_WORD),QVariant::fromValue(static_cast<MODBUS_WORD>(0)));
        });
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
