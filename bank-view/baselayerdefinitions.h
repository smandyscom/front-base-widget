#ifndef BASELAYERDEFINITIONS_H
#define BASELAYERDEFINITIONS_H

#include <QObject>
#include <QAbstractTransition>
#include <QVariant>
#include <QModbusDataUnit>

//address-QVariant pair

namespace BaseLayer {

#define WORD qint16
#define LONG qint32
#define BIT  bool

//!
//! \brief The AbstractAddress class
//! 32Bits addressing mode
class AbstractAddress
{
public :
    AbstractAddress(uint address=0):address(address){}
    //!
    //! \brief AbstractAddress
    //! \param source
    //! copy constructor
    AbstractAddress(const AbstractAddress& source)
    {
        address = source.address;
    }

    virtual uint toBitwiseMask() const
    {
        return 0;
    }
    uint readAddress() const {return address;}
    uint getAddress() const{return address;}

protected :
    uint address;
};

inline bool operator==(const AbstractAddress& lhp,const AbstractAddress& rhp)
{
    return lhp.readAddress() == rhp.readAddress();
}
inline uint qHash(const AbstractAddress &key, uint seed)
{
    return key.readAddress();
}
inline bool operator !=(const AbstractAddress& lhp,const AbstractAddress& rhp)
{
    return !(lhp==rhp);
}
//!
//! \brief The ModbusDriverAddress struct
//! 32Bits addressing mode
class ModbusDriverAddress : public AbstractAddress
{
protected:
    quint8* channelAddress;
    quint8* registerTypeBitIndex;//upper half byte: coil,input,holding , lower half bytpe: bit index
    quint16* registerAddress;
public:
    ModbusDriverAddress(uint address=0):AbstractAddress(address)
    {
        channelAddress = &reinterpret_cast<quint8*>(&this->address)[3]; //highest byte
        registerTypeBitIndex = &reinterpret_cast<quint8*>(&this->address)[2];
        registerAddress = &reinterpret_cast<quint16*>(&this->address)[0];
    }
    QModbusDataUnit::RegisterType getRegisterType() const{
        return QModbusDataUnit::RegisterType(*registerTypeBitIndex>>4);
    }
    void setRegisterType(QModbusDataUnit::RegisterType type){
        *registerTypeBitIndex = (*registerTypeBitIndex & 0x0F) + (type << 4);}

    quint8 getBitIndex() const{return (*registerTypeBitIndex) & 0x0f;}
    void setBitIndex(quint8 index){*registerTypeBitIndex = (*registerTypeBitIndex & 0xf0) | (index & 0x0f);}

    quint8 getChannelAddress() const {return *channelAddress;}
    quint16 getRegisterAddress() const {return *registerAddress;}


    virtual uint toBitwiseMask() const
    {
        return 0x0001 << (*registerAddress & 0x000f);
    }
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
//Q_DECLARE_METATYPE(BaseLayer::DummyDriverAddress)

#endif // BASELAYERDEFINITIONS_H
