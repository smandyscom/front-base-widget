#ifndef BASELAYERDEFINITIONS_H
#define BASELAYERDEFINITIONS_H

#include <QObject>
#include <QAbstractTransition>
#include <QVariant>


//address-QVariant pair

namespace BaseLayer {

#define WORD qint16
#define LONG qint32
#define BIT  bool

//!
//! \brief The AbstractAddress class
//! 32Bits addressing mode
struct AbstractAddress
{
public :
    virtual quint32 toBitwiseMask()
    {
        return 0;
    }

};

inline bool operator==(const AbstractAddress& lhp,const AbstractAddress& rhp)
{
    return *reinterpret_cast<const uint*>(&lhp) == *reinterpret_cast<const uint*>(&rhp);
}
inline uint qHash(const AbstractAddress &key, uint seed)
{
    return *reinterpret_cast<const uint*>(&key);
}
inline bool operator !=(const AbstractAddress& lhp,const AbstractAddress& rhp)
{
    return !(lhp==rhp);
}
//!
//! \brief The ModbusDriverAddress struct
//! 32Bits addressing mode
struct ModbusDriverAddress : public AbstractAddress
{
    quint8 channelAddress;
    quint8 registerTypeBitIndex;//upper half byte: coil,input,holding , lower half bytpe: bit index
    quint16 registerAddress;

    virtual quint32 toBitwiseMask()
    {
        return 0x01 << (registerAddress & 0x0F);
    }
};

//other driver addressing mode
struct DummyDriverAddress : public AbstractAddress
{
  quint32 reserved;
};

//!
//! \brief AddressValueBinding
//! QVarariable had contended type info
typedef QPair<AbstractAddress,QVariant> AddressValueBinding;

//!
//! \brief The UpdateEvent struct
//!
struct UpdateEvent: public QEvent
{
    UpdateEvent(const AbstractAddress address,const QVariant value):
        QEvent(UpdateEvent::typeCode),
        address(address),
        value(value){}

    AbstractAddress address;
    QVariant value;

    static QEvent::Type typeCode;
};

QEvent::Type UpdateEvent::typeCode = QEvent::Type(QEvent::registerEventType());

//!
//! \brief The ValueTransition class
//!
class ValueTransition : public QAbstractTransition
{


public:
    //Q_OBJECT
    enum Detection : quint8
    {
        BIT_STATE_ON,
        BIT_STATE_OFF,
        VALUE_UPDATED
    };

    ValueTransition(AbstractAddress address,Detection action,QObject *parent = nullptr):
    __address(address),
    __detection(action){}
protected:
    virtual bool eventTest(QEvent *event)
    {
        //casting to update event
        if (event->type() != UpdateEvent::typeCode){
            return false; //not update event
        }

        UpdateEvent* ue = static_cast<UpdateEvent*>(event);

        if (ue->address != __address)
            return false; //not cared address

        switch (__detection) {
        case BIT_STATE_ON:
             return (ue->value.value<qint32>() & ue->address.toBitwiseMask()) > 0;
            break;
        case BIT_STATE_OFF:
            return (ue->value.value<qint32>() & ue->address.toBitwiseMask()) == 0;
            break;
        case VALUE_UPDATED:
            return true;
            break;
        default:
            break;
        }

        return false;
    }
    virtual void onTransition(QEvent*){}


    AbstractAddress __address;
    Detection __detection;
};

}


Q_DECLARE_METATYPE(BaseLayer::ModbusDriverAddress)
Q_DECLARE_METATYPE(BaseLayer::DummyDriverAddress)

#endif // BASELAYERDEFINITIONS_H
