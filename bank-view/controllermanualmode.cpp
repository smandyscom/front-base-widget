#include "controllermanualmode.h"

ControllerManualMode::ControllerManualMode(QObject *parent) :
    QStateMachine(parent)
{
    //!
    //! \brief s1
    //!
    QState* s1 = new QState(this);
    QState* s2 = new QState(this);
    QState* s3 = new QState(this);

    //!
    //! \brief engagedPLCOff
    //! Common transition
    ValueTransition* engagedPLCOff = new ValueTransition(AbstractAddress(ENGAGED_PLC),ValueTransition::BIT_STATE_OFF,this);
    engagedPLCOff->setTargetState(s1);
    QList<QState*> states({s1,s2,s3});
    foreach (QState* s, states)
    {
        //!
        //! Link common transition
        //!
        s->addTransition(engagedPLCOff);

        connect(s,&QState::entered,this,[this](){
            //!
            //! trigger read action
            emit requireReadData(AbstractAddress(STATUS_WORD),QVariant::fromValue(static_cast<WORD>(0)));
        });
    }


    //!
    //! s1
    s1->addTransition(this,SIGNAL(triggerOperation()),s2);// when user triggered
    connect(s1,&QState::exited,[this](){
        //!
        //! commit block if need
        switch (commitBlockCache.mode) {
        case DOWNLOAD:
            emit requireWriteData(AbstractAddress(DATA_BLOCK_HEAD),dataBlockCache);
            break;
        default:
            break;
        }

        emit requireWriteData(AbstractAddress(COMMIT_BLOCK),QVariant::fromValue(commitBlockCache));
        //set Run on
        emit requireWriteData(AbstractAddress(RUN),QVariant::fromValue(true));
    });
    //!
    //! s2
    ValueTransition* doneOn = new ValueTransition(AbstractAddress(DONE),ValueTransition::BIT_STATE_ON,this);
    doneOn->setTargetState(s3);
    s2->addTransition(doneOn); //when DONE on
    connect(s2,&QState::exited,[this](){
        //!
        //! read out block if need
        switch (commitBlockCache.mode) {
        case UPLOAD:
            dataBlockCache.setValue(dummyFcb); //should read full size
            emit requireReadData(AbstractAddress(DATA_BLOCK_HEAD),dataBlockCache);
            break;
        default:
            break;
        }

        //set RUN off
        emit requireWriteData(AbstractAddress(RUN),QVariant::fromValue(false));
    });
    //!
    //! s3
    ValueTransition* doneOff = new ValueTransition(AbstractAddress(DONE),ValueTransition::BIT_STATE_OFF,this);
    doneOff->setTargetState(s1);
    s3->addTransition(doneOff); //when DONE off

}
