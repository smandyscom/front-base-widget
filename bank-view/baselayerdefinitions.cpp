#include<baselayerdefinitions.h>
using namespace BaseLayer;

QEvent::Type updateEventTypeCode = QEvent::Type(QEvent::registerEventType());

UpdateEvent::UpdateEvent(AbstractAddress* address,const QVariant value):
    QEvent(updateEventTypeCode),
    address(address),
    value(value)
{

}

bool ValueTransition::eventTest(QEvent *event)
{
    //casting to update event
    if (event->type() != updateEventTypeCode){
        return false; //not update event
    }

    UpdateEvent* ue = static_cast<UpdateEvent*>(event);

    //uint a1 = ue->address->getAddress();
    //uint a2 = address->getAddress();

    if (*ue->address != *address)
        return false; //not cared address

    switch (detection) {
    case BIT_STATE_ON:
         return (ue->value.value<qint32>() & ue->address->toBitwiseMask()) > 0;
        break;
    case BIT_STATE_OFF:
        return (ue->value.value<qint32>() & ue->address->toBitwiseMask()) == 0;
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
