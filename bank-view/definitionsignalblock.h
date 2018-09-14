#ifndef DEFINITIONSIGNALBLOCK_H
#define DEFINITIONSIGNALBLOCK_H

#include<QObject>

#include<definitionbasicblocks.h>
#include<definitionauxiliarykeys.h>

#include<abstractsqltableadpater.h>

using namespace DEF_BASIC_DIMENSION;

class SignalMonitor :
        public AbstractDataBlock
{
    Q_OBJECT
public:
    SignalMonitor(QObject* parent=nullptr) :
        AbstractDataBlock(parent){}
    SignalMonitor(MODBUS_U_WORD* anchor,QObject* parent=nullptr) :
        AbstractDataBlock(anchor,parent){}

    enum OffsetMonitor
    {
        OFFSET_MONITOR_WIDTH_ON_COUNT=3,
        OFFSET_MONITOR_WIDTH_OFF_COUNT=4,
        OFFSET_MONITOR_WORD_OUT=5,
    };
    Q_ENUM(OffsetMonitor)
    enum WordOutBits
    {
        BIT_0_DEBOUNCED_ON=0x00000+OFFSET_MONITOR_WORD_OUT,
        BIT_1_DEBOUNCED_OFF=0x10000+OFFSET_MONITOR_WORD_OUT,
        BIT_2_RAW=0x20000+OFFSET_MONITOR_WORD_OUT,
    };
    Q_ENUM(WordOutBits)

    QVariant Value(uint key) const Q_DECL_OVERRIDE
    {
        switch (key) {
        case OFFSET_MONITOR_WIDTH_ON_COUNT:
        case OFFSET_MONITOR_WIDTH_OFF_COUNT:
            return QVariant::fromValue(getData<MODBUS_U_WORD>(key) * Dimension->value(TIME));
            break;
        default:
            return QVariant::fromValue(Bit(key));
            break;
        }
    }
};

class SignalContext :
        public SignalMonitor
{
    Q_OBJECT
public:
    SignalContext(QObject* parent=nullptr) :
        SignalMonitor(parent){}
    SignalContext(MODBUS_U_WORD* anchor,QObject* parent=nullptr) :
        SignalMonitor(anchor,parent){}

    enum OffsetContext
    {
        OFFSET_CONTEXT_SIGNAL_ADDRESS=0,
        OFFSET_CONTEXT_WIDTH_ON_SET=1,
        OFFSET_CONTEXT_WIDTH_OFF_SET=2,
    };
    Q_ENUM(OffsetContext)

    QVariant Value(uint key) const Q_DECL_OVERRIDE
    {
        switch (key) {
        case OFFSET_CONTEXT_SIGNAL_ADDRESS:
            return QVariant::fromValue(getData<MODBUS_U_WORD>(key));
        case OFFSET_CONTEXT_WIDTH_ON_SET:
        case OFFSET_CONTEXT_WIDTH_OFF_SET:
            return QVariant::fromValue(Dimension->value(TIME) *  getData<MODBUS_U_WORD>(key));
            break;
        default:
            return SignalMonitor::Value(key);
            break;
        }
    }
    void Value(uint key,QVariant value) Q_DECL_OVERRIDE
    {
        switch (key) {
        case OFFSET_CONTEXT_SIGNAL_ADDRESS:
            setData(key,value.value<MODBUS_U_WORD>());
            break;
        case OFFSET_CONTEXT_WIDTH_ON_SET:
        case OFFSET_CONTEXT_WIDTH_OFF_SET:
            setData(key,static_cast<MODBUS_U_WORD>(value.toReal() / Dimension->value(TIME)));
            break;
        default:
            break;
        }
    }
};

namespace SignalBlock {

Q_NAMESPACE
enum DataBaseHeaders
{
    ID=INVALID_INDEX-1,
    NAME=INVALID_INDEX-2,
    REGION=INVALID_INDEX-3,
    //!
    PLC_ADDRESS = SignalContext::OFFSET_CONTEXT_SIGNAL_ADDRESS,
    WIDTH_ON_SET = SignalContext::OFFSET_CONTEXT_WIDTH_ON_SET,
    WIDTH_OFF_SET = SignalContext::OFFSET_CONTEXT_WIDTH_OFF_SET,
};
Q_ENUM_NS(DataBaseHeaders)


}

#endif // DEFINITIONSIGNALBLOCK_H
