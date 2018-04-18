#include "controllermainpanel.h"

ControllerMainPanel::ControllerMainPanel(QObject *parent) : QObject(parent)
{
    __channel = ModbusChannel::Instance();

    //! Very first shot
    __channel->beginAccess<AbstractDataBlock>(ModbusDriverAddress(UnitContextBlock::LUID_PARENT));

    //! Link
    connect(__channel,&ModbusChannel::raiseUpdateEvent,this,&ControllerMainPanel::onReply);
}

//!
//! \brief ControllerMainPanel::onReply
//! \param event
//!
void ControllerMainPanel::onReply(UpdateEvent *event)
{
    switch (event->address) {
    case UnitContextBlock::LUID_PARENT:
        QTimer::singleShot(100,this,[=](){
            //! Schedualing next polling
            __channel->beginAccess<AbstractDataBlock>(ModbusDriverAddress(UnitContextBlock::LUID_PARENT));
        });
        break;
    default:
        break;
    }
}

ControllerMainPanel* ControllerMainPanel::Instance()
{
    if(__instace ==nullptr)
        __instace = new ControllerMainPanel();
    return __instace;
}

ControllerMainPanel* ControllerMainPanel::__instace = nullptr;
