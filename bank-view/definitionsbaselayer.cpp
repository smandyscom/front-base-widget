#include<definitionsbaselayer.h>
using namespace BaseLayer;

QEvent::Type updateEventTypeCode = QEvent::Type(QEvent::registerEventType());

UpdateEvent::UpdateEvent(const ADDRESS_MODE &address, const QVariant value):
    QEvent(updateEventTypeCode),
    address(address.getAddress()),
    value(value)
{
    //!
}

bool ValueTransition::eventTest(QEvent *event)
{
    //casting to update event
    if (event->type() != updateEventTypeCode){
        return false; //not update event
    }

    UpdateEvent* ue = static_cast<UpdateEvent*>(event);

    //! Match channel and register address
    if ((ue->address & 0xff00ffff) != (__address & 0xff00ffff))
        return false; //not cared address

    bool logic = (ue->value.value<qint16>() & __bitAccessor) > 0;
    switch (detection) {
    case BIT_STATE_ON:
         return logic;
        break;
    case BIT_STATE_OFF:
        return !logic;
        break;
    case VALUE_UPDATED:
        return true;
        break;
    default:
        break;
    }

    return false;
}

void ValueTransition::onTransition(QEvent*)
{

}


ModbusDriverAddress::ModbusDriverAddress(uint address):AbstractAddress(address)
{
    channelAddress = &reinterpret_cast<quint8*>(&this->address)[3]; //highest byte
    registerTypeBitIndex = &reinterpret_cast<quint8*>(&this->address)[2];
    registerAddress = &reinterpret_cast<quint16*>(&this->address)[0];
}
ModbusDriverAddress::ModbusDriverAddress(const ModbusDriverAddress &address)
    : AbstractAddress(address)
{
    channelAddress = &reinterpret_cast<quint8*>(&this->address)[3]; //highest byte
    registerTypeBitIndex = &reinterpret_cast<quint8*>(&this->address)[2];
    registerAddress = &reinterpret_cast<quint16*>(&this->address)[0];
}

ModbusDriverAddress::ModbusDriverAddress(quint8 channel, quint8 bitIndex, quint16 ___registerAddress)
{
    channelAddress = &reinterpret_cast<quint8*>(&this->address)[3]; //highest byte
    registerTypeBitIndex = &reinterpret_cast<quint8*>(&this->address)[2];
    registerAddress = &reinterpret_cast<quint16*>(&this->address)[0];
    //
    setChannel(channel);
    setBitIndex(bitIndex);
    setRegisterAddress(___registerAddress);
}
