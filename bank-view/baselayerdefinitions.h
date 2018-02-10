#ifndef BASELAYERDEFINITIONS_H
#define BASELAYERDEFINITIONS_H

#include <QObject>
#include <QAbstractTransition>
#include <QVariant>
#include <QModbusDataUnit>
#include <QEvent>
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
    AbstractAddress(const AbstractAddress& source):address(source.address){}

    virtual uint toBitwiseMask() const
    {
        return 0;
    }
    uint getAddress() const{return address;}

protected :
    uint address;
};

inline bool operator==(const AbstractAddress& lhp,const AbstractAddress& rhp)
{
    return lhp.getAddress() == rhp.getAddress();
}
inline uint qHash(const AbstractAddress &key, uint seed)
{
    return key.getAddress();
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
    ModbusDriverAddress(uint address=0);
    //!
    //! \brief ModbusDriverAddress
    //! \param address
    //! copy constructor
    ModbusDriverAddress(const ModbusDriverAddress& address);

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
        return 0x0001 << (*registerTypeBitIndex & 0x0f);
    }
};

//!
//! \brief AddressValueBinding
//! QVarariable had contended type info
typedef QPair<AbstractAddress,QVariant> AddressValueBinding;


//extern QEvent::Type updateEventTypeCode;// = QEvent::Type(QEvent::registerEventType());
//!
//! \brief The UpdateEvent struct
//!
class UpdateEvent: public QEvent
{
public :

    UpdateEvent(const AbstractAddress& address,const QVariant value);

    const uint address;
    const uint bitMask;
    QVariant value;
};


//!
//! \brief The ValueTransition class
//! Offering the transition for state machine
//! Sort of message/event filter
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

    ValueTransition(const AbstractAddress& address,Detection action,QObject *parent = nullptr):
    address(address.getAddress()),
    detection(action){}

protected:
    virtual bool eventTest(QEvent *event);
    virtual void onTransition(QEvent*);


    const uint address;
    Detection detection;
};

}


Q_DECLARE_METATYPE(BaseLayer::ModbusDriverAddress)
//Q_DECLARE_METATYPE(BaseLayer::DummyDriverAddress)

#endif // BASELAYERDEFINITIONS_H