#include "controllermanualmode.h"

ControllerManualMode::ControllerManualMode(QObject *parent) :
    QStateMachine(parent)
{
    //! Channel initialize
    channel = ModbusChannel::Instance();

    //!
    //! \brief s1
    //!
    QState* s1 = new QState(this);
    QState* s2 = new QState(this);
    QState* s3 = new QState(this);

    //!
    //! \brief engagedPLCOff
    //! Common transition
    ValueTransition* engagedPLCOff = new ValueTransition(AbstractAddress(ENGAGED_PLC),ValueTransition::BIT_STATE_OFF);
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
            emit requireReadData(AbstractAddress(STATUS_WORD),QVariant::fromValue(static_cast<MODBUS_WORD>(0)));
        });
    }


    //!
    //! s1
    s1->addTransition(this,SIGNAL(operationTriggered()),s2);// when user triggered
    connect(s1,&QState::entered,[this](){emit operationPerformed();}); //inform required operation had performed
    connect(s1,&QState::exited,[this](){
        //!
        //! commit block if need
        switch (__commitOption.Mode()) {
        case CommitBlock::MODE_DOWNLOAD:
            //! Always write-in full-size
            emit requireWriteData(AbstractAddress(DATA_BLOCK_HEAD),QVariant::fromValue(__commandBlock));
            break;
        default:
            break;
        }

        emit requireWriteData(AbstractAddress(COMMIT_BLOCK),QVariant::fromValue(__commitOption));
        emit requireWriteData(AbstractAddress(RUN),QVariant::fromValue(true));//set Run on
    });
    //!
    //! s2
    ValueTransition* doneOn = new ValueTransition(AbstractAddress(DONE),ValueTransition::BIT_STATE_ON);
    ValueTransition* doneNotOn = new ValueTransition(AbstractAddress(DONE),ValueTransition::BIT_STATE_OFF);
    doneOn->setTargetState(s3);
    doneNotOn->setTargetState(s2); //self transition

    s2->addTransition(doneOn); //when DONE on
    s2->addTransition(doneNotOn); //when DONE off

    connect(s2,&QState::exited,[this](){
        //!
        //! read out block if need
        switch (__commitOption.Mode()) {
        case CommitBlock::MODE_UPLOAD:
            //! should read full size
            emit requireReadData(AbstractAddress(DATA_BLOCK_HEAD),QVariant::fromValue(__commandBlock));
            break;
        default:
            break;
        }

        //set RUN off
        emit requireWriteData(AbstractAddress(RUN),QVariant::fromValue(false));
    });
    //!
    //! s3
    ValueTransition* doneOff = new ValueTransition(AbstractAddress(DONE),ValueTransition::BIT_STATE_OFF);
    ValueTransition* doneNotOff = new ValueTransition(AbstractAddress(DONE),ValueTransition::BIT_STATE_ON);

    doneOff->setTargetState(s1);
    doneNotOff->setTargetState(s3); //self transition

    s3->addTransition(doneOff); //when DONE off
    s3->addTransition(doneNotOff); //when DONE on

}

//!
//! \brief ControllerManualMode::onMonitorBlockReply
//! \param event
//! intercept monitor block updating event
void ControllerManualMode::onMonitorBlockReply(UpdateEvent *event)
{
    switch (event->address) {
    case POS_COMMAND:
        //! keep polling monitor status
        emit requireReadData(AbstractAddress(POS_COMMAND),QVariant::fromValue(__monitorBlock));
        break;
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
