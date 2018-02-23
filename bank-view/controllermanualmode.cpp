#include "controllermanualmode.h"

ControllerManualMode::ControllerManualMode(QObject *parent) : QObject(parent)
{
    //!State machine construction
    //!
    trunsactionController = new QStateMachine(this);

    QState* s1 = new QState(trunsactionController);
    QState* s2 = new QState(trunsactionController);
    QState* s3 = new QState(trunsactionController);

    //!
    //! \brief engagedPLCOff
    //! Common transition
    ValueTransition* engagedPLCOff = new ValueTransition(AbstractAddress(ENGAGED_PLC),ValueTransition::BIT_STATE_OFF,this);
    engagedPLCOff->setTargetState(s1);
    QState* states[] = {s1,s2,s3};
    foreach (QState* s, states)
    {
        //!
        //! Link common transition
        //!
        s->addTransition(engagedPLCOff);

        connect(s,&QState::entered,this,[this](){
            //!
            //! trigger read action
            emit requireReadData(AbstractAddress(ENGAGED_PLC),QVariant(static_cast<WORD>(0)));
        });
    }


    //!
    //! s1
    s1->addTransition(this,,s2);// when user triggered
    connect(s1,&QState::exited,[](){
        //set RUN on
        emit requireWriteData(AbstractAddress(RUN),QVariant(true));
    });
    //!
    //! s2
    ValueTransition* doneOn = new ValueTransition(AbstractAddress(DONE),ValueTransition::BIT_STATE_ON,this);
    doneOn->setTargetState(s3);
    s2->addTransition(doneOn); //when DONE on
    connect(s2,&QState::exited,[](){
        //set RUN off
        emit requireWriteData(AbstractAddress(RUN),QVariant(false));
    });
    //!
    //! s3
    ValueTransition* doneOff = new ValueTransition(AbstractAddress(DONE),ValueTransition::BIT_STATE_OFF,this);
    doneOff->setTransitionType(s4);
    s3->addTransition(doneOff); //when DONE off
    connect(s3,&QState::exited,[](){
       //do Block read
        emit requireWriteData(AbstractAddress(BLOCK_HEAD),QVariant(false)); //TODO
    });
    //!
    //! s4
    ValueTransition* blockReadDone = new ValueTransition(AbstractAddress(BLOCK_HEAD),ValueTransition::VALUE_UPDATED,this);
    engagedPLCOff->setTargetState(s1);
    s4->addTransition(blockReadDone);
    connect(s4,&QState::exited,[](){
        //read out Block
    });



}

