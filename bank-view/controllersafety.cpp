#include "controllersafety.h"

ControllerSafety::ControllerSafety(QObject *parent) : QObject(parent)
{
    //! Very first shot
    __channel = ModbusChannel::Instance();
    connect(__channel,&ModbusChannel::readReply,this,&ControllerSafety::onReply);
    __channel->beginAccess<MODBUS_U_WORD>(ModbusDriverAddress(MONITOR)); //4-words
    //!
    __channel->beginAccess<bool>(ModbusDriverAddress(INTERLOCK_BYPASS));
}

void ControllerSafety::onReply()
{
    switch (__channel->CachedReplyAddress().getAddress()) {
    case MONITOR:
        QTimer::singleShot(100,[=](){
            __channel->beginAccess<MODBUS_U_WORD>(ModbusDriverAddress(MONITOR));
        });
        break;
    case INTERLOCK_BYPASS:
        emit initialized();
        break;
    default:
        break;
    }
}

ControllerSafety* ControllerSafety::Instance()
{
    if(__instance == nullptr)
        __instance = new ControllerSafety();
    return __instance;
}

ControllerSafety* ControllerSafety::__instance = nullptr;
