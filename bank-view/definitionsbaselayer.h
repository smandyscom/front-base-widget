#ifndef BASELAYERDEFINITIONS_H
#define BASELAYERDEFINITIONS_H

#include <QObject>
#include <QAbstractTransition>
#include <QVariant>
#include <QModbusDataUnit>
#include <QEvent>

namespace BaseLayer {

#define MODBUS_S_WORD qint16
#define MODBUS_U_WORD quint16

#define MODBUS_S_LONG qint32
#define MODBUS_U_LONG quint32

#define MODBUS_S_QUAD qint64
#define MODBUS_U_QUAD quint64

#define MODBUS_ADDRESS quint16

#define ADDRESS_MODE uint
#define ADDRESS_CLIENT_ID(x) reinterpret_cast<quint8*>(&x)[3]
#define ADDRESS_BIT_ACCESSOR(x) 0x0001 << reinterpret_cast<quint8*>(&x)[2]
#define ADDRESS_REGISTER(x) reinterpret_cast<quint16*>(&x)[0]
//32bits[4 bytes] , client id[1 byte],bit accessor(16bits)[1 byte] , register address (16bits for each register)[2 byte]


////!
////! \brief The AbstractAddress class
////! 32Bits addressing mode
//class AbstractAddress
//{
//public :
//    AbstractAddress(uint address=0):address(address){}
//    //!
//    //! \brief AbstractAddress
//    //! \param source
//    //! copy constructor
//    AbstractAddress(const AbstractAddress& source):address(source.address){}

//    virtual uint toBitwiseMask() const
//    {
//        return 0;
//    }
//    uint getAddress() const{return address;}

//protected :
//	//ff				f		f					ffff
//	//(client index)	(type)	(bit access index)	(register address)
//    uint address; //still 32bit addressing mode
//};

//inline bool operator==(const AbstractAddress& lhp,const AbstractAddress& rhp)
//{
//    return lhp.getAddress() == rhp.getAddress();
//}
//inline bool operator !=(const AbstractAddress& lhp,const AbstractAddress& rhp)
//{
//    return !(lhp==rhp);
//}
//inline bool operator <(const AbstractAddress& lhp,const AbstractAddress& rhp)
//{
//    return lhp.getAddress()<rhp.getAddress();
//}



////!
////! \brief The ModbusDriverAddress struct
////! 32Bits addressing mode
//class ModbusDriverAddress : public AbstractAddress
//{
//protected:
//    quint8* channelAddress;
//    quint8* registerTypeBitIndex;//upper half byte: coil,input,holding , lower half bytpe: bit index
//    quint16* registerAddress;
//public:
//    ModbusDriverAddress(uint address=0);
//    //!
//    //! \brief ModbusDriverAddress
//    //! \param address
//    //! copy constructor
//    ModbusDriverAddress(const ModbusDriverAddress& address);
//    ModbusDriverAddress(quint8 channel,quint8 bitIndex,quint16 registerAddress);

//    QModbusDataUnit::RegisterType getRegisterType() const{
//        return QModbusDataUnit::RegisterType(*registerTypeBitIndex>>4);
//    }
//    void setRegisterType(QModbusDataUnit::RegisterType type){
//        *registerTypeBitIndex = (*registerTypeBitIndex & 0x0F) + (type << 4);}

//    quint8 getBitIndex() const{return (*registerTypeBitIndex) & 0x0f;}
//    void setBitIndex(quint8 index){*registerTypeBitIndex = (*registerTypeBitIndex & 0xf0) | (index & 0x0f);}

//    quint8 getChannel() const {return *channelAddress;}
//    quint16 getRegisterAddress() const {return *registerAddress;}

//    void setChannel(quint8 __channelAddress){(*channelAddress) = __channelAddress;}
//    void setRegisterAddress(quint16 __registerAddress){(*registerAddress)=__registerAddress;}

//    virtual uint toBitwiseMask() const
//    {
//        return 0x0001 << (*registerTypeBitIndex & 0x0f);
//    }
//};

//!
//! \brief The UpdateEvent struct
//!
class UpdateEvent: public QEvent
{
public :

    UpdateEvent(const ADDRESS_MODE& address,const QVariant value);

    const ADDRESS_MODE address;
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

    ValueTransition(const ADDRESS_MODE& address,Detection action):
        __address(ADDRESS_REGISTER(address)),
        __bitAccessor(ADDRESS_BIT_ACCESSOR(address)),
        detection(action)
    {}

protected:
    virtual bool eventTest(QEvent *event);
    virtual void onTransition(QEvent*);


    const quint16 __address;
    const quint16 __bitAccessor;
    Detection detection;
};

}//namespace


#endif // BASELAYERDEFINITIONS_H
