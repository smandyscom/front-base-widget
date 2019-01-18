#ifndef UNITBLOCKSDEFINITION_H
#define UNITBLOCKSDEFINITION_H

#include<definitionbasicblocks.h>

#include<abstractsqltableadpater.h>

#include<definitionauxiliarykeys.h>
using namespace DEF_BASIC_DIMENSION;

class UnitMonitorBlock:
        public AbstractDataBlock
{
    Q_OBJECT
public:
    UnitMonitorBlock(QObject* parent=nullptr) :
        AbstractDataBlock(parent)
    {

    }
    UnitMonitorBlock(MODBUS_U_WORD* anchor,QObject* parent=nullptr) :
        AbstractDataBlock(anchor,parent)
    {

    }

    enum OffsetMonitor
    {
        OFFSET_MONITOR_STATE=0x00000001,
        OFFSET_MONITOR_ALARM_CODE=0x00000002,
        OFFSET_MONITOR_NEXT_STATE=0x00000004,
        OFFSET_MONITOR_TRANSITION_WORD=0x00000005,
        OFFSET_MONITOR_WORKING_TIMER_COUNTER=0x00000008,
        OFFSET_MONITOR_WORKING_TIMER_CACHE=0xA,
    };
    Q_ENUM(OffsetMonitor)
    enum TransitionBits
    {
        BIT_0_SERIAL_TRANSITON=0 + OFFSET_MONITOR_TRANSITION_WORD,
        BIT_9_TEMP_CONDITION_1 = 0x080000 + OFFSET_MONITOR_TRANSITION_WORD,
        BIT_10_TEMP_CONDITION_2 = 0x090000 + OFFSET_MONITOR_TRANSITION_WORD,
        BIT_11_TEMP_CONDITION_3 = 0x0A0000 + OFFSET_MONITOR_TRANSITION_WORD,
        BIT_12_TEMP_CONDITION_4 = 0x0B0000 + OFFSET_MONITOR_TRANSITION_WORD,
        BIT_13_TEMP_CONDITION_5 = 0x0C0000 + OFFSET_MONITOR_TRANSITION_WORD,
        BIT_14_TEMP_CONDITION_6 = 0x0D0000 + OFFSET_MONITOR_TRANSITION_WORD,
        BIT_15_TEMP_CONDITION_7 = 0x0E0000 + OFFSET_MONITOR_TRANSITION_WORD,
        BIT_16_TEMP_CONDITION_8 = 0x0F0000 + OFFSET_MONITOR_TRANSITION_WORD,
    };
    Q_ENUM(TransitionBits)

    QVariant Value(uint key) const Q_DECL_OVERRIDE
    {
        switch (key) {
        case OFFSET_MONITOR_STATE:
        case OFFSET_MONITOR_ALARM_CODE:
        case OFFSET_MONITOR_NEXT_STATE:
        case OFFSET_MONITOR_WORKING_TIMER_COUNTER:
            return AbstractDataBlock::Value(key);
            break;
        case OFFSET_MONITOR_WORKING_TIMER_CACHE:
            return QVariant::fromValue(getData<MODBUS_U_LONG>(key) * Dimension->value(TIME));
            break;
        default:
            return QVariant::fromValue(Bit(key));
            break;
        }
    }

};
//Q_DECLARE_METATYPE(UnitMonitorBlock)

class UnitOperationBlock:
        public UnitMonitorBlock
{
    Q_OBJECT
public:
    UnitOperationBlock(QObject* parent=nullptr) :
        UnitMonitorBlock(parent)
    {

    }
    UnitOperationBlock(MODBUS_U_WORD* anchor,QObject* parent=nullptr) :
        UnitMonitorBlock(anchor,parent)
    {

    }

    enum OffsetOperation
    {
        OFFSET_OPERATION_CONTROL_WORD=3,
    };
    Q_ENUM(OffsetOperation)
    enum ControlBits
    {
        OFFSET_UOB_EN_STEP=0x00000+OFFSET_OPERATION_CONTROL_WORD,
        OFFSET_UOB_TRIG_STEP=0x10000+OFFSET_OPERATION_CONTROL_WORD,
        OFFSET_UOB_STATE_PAUSE=0x20000+OFFSET_OPERATION_CONTROL_WORD,
        OFFSET_UOB_IS_BYPASSED=0x30000+OFFSET_OPERATION_CONTROL_WORD,
        OFFSET_UOB_WORKING_OVERRIDE=0x40000+OFFSET_OPERATION_CONTROL_WORD,
        OFFSET_UOB_MATERIAL_OVERRIDE=0x50000+OFFSET_OPERATION_CONTROL_WORD,
    };
    Q_ENUM(ControlBits)

    void Value(uint key,QVariant value) Q_DECL_OVERRIDE
    {
        Bit(key,value.toBool());
    }
    QVariant Value(uint key) const Q_DECL_OVERRIDE
    {
        return UnitMonitorBlock::Value(key);
    }
};
//Q_DECLARE_METATYPE(UnitOperationBlock)

class UnitContextBlock :
        public UnitOperationBlock
{
    Q_OBJECT
public:
    UnitContextBlock(QObject* parent=nullptr) :
        UnitOperationBlock(parent)
    {

    }
    UnitContextBlock(MODBUS_U_WORD* anchor,QObject* parent=nullptr) :
        UnitOperationBlock(anchor,parent)
    {

    }

    enum OffsetContext : uint
    {
        OFFSET_CONTEXT_LUID_PARENT=0,
        OFFSET_CONTEXT_LSID_BASE=6,
        OFFSET_CONTEXT_LSID_COUNT=7,
        //! Extension fields
        OFFSET_CONTEXT_F16=16,
        OFFSET_CONTEXT_F18=18,
        OFFSET_CONTEXT_F20=20,
        OFFSET_CONTEXT_F22=22,
        OFFSET_CONTEXT_F24=24,
        OFFSET_CONTEXT_F26=26,
        OFFSET_CONTEXT_F28=28,
        OFFSET_CONTEXT_F30=30,
    };
    Q_ENUM(OffsetContext)

    void Value(uint key, QVariant value) Q_DECL_OVERRIDE
    {
        switch (key) {
        case OFFSET_CONTEXT_LUID_PARENT:
        case OFFSET_CONTEXT_LSID_BASE:
        case OFFSET_CONTEXT_LSID_COUNT:
            setData(key,value.value<MODBUS_U_WORD>());
            break;
        case OFFSET_CONTEXT_F16:
        case OFFSET_CONTEXT_F18:
        case OFFSET_CONTEXT_F20:
        case OFFSET_CONTEXT_F22:
        case OFFSET_CONTEXT_F24:
        case OFFSET_CONTEXT_F26:
        case OFFSET_CONTEXT_F28:
        case OFFSET_CONTEXT_F30:
            setData(key,value.value<MODBUS_U_LONG>());
            break;
        default:
            UnitOperationBlock::Value(key,value);
            break;
        }
    }
    QVariant Value(uint key) const Q_DECL_OVERRIDE
    {
        switch (key) {
        case OFFSET_CONTEXT_LUID_PARENT:
        case OFFSET_CONTEXT_LSID_BASE:
        case OFFSET_CONTEXT_LSID_COUNT:
            return getData<MODBUS_U_WORD>(key);
            break;
        case OFFSET_CONTEXT_F16:
        case OFFSET_CONTEXT_F18:
        case OFFSET_CONTEXT_F20:
        case OFFSET_CONTEXT_F22:
        case OFFSET_CONTEXT_F24:
        case OFFSET_CONTEXT_F26:
        case OFFSET_CONTEXT_F28:
        case OFFSET_CONTEXT_F30:
            return getData<MODBUS_U_LONG>(key);
            break;
        default:
            return UnitOperationBlock::Value(key);
            break;
        }
    }
};
//Q_DECLARE_METATYPE(UnitContextBlock)

namespace UnitBlock {
Q_NAMESPACE
enum DataBaseHeaders
{
    ID=INVALID_INDEX-1,
    TYPE=INVALID_INDEX-2,
    TYPE_N=INVALID_INDEX-3,
    NAME=INVALID_INDEX-4,
    //!
    LUID_PARENT=UnitContextBlock::OFFSET_CONTEXT_LUID_PARENT,
    LSID_BASE=UnitContextBlock::OFFSET_CONTEXT_LSID_BASE,
    LSID_COUNT=UnitContextBlock::OFFSET_CONTEXT_LSID_COUNT,
    F16=UnitContextBlock::OFFSET_CONTEXT_F16,
    F18=UnitContextBlock::OFFSET_CONTEXT_F18,
    F20=UnitContextBlock::OFFSET_CONTEXT_F20,
    F22=UnitContextBlock::OFFSET_CONTEXT_F22,
    F24=UnitContextBlock::OFFSET_CONTEXT_F24,
    F26=UnitContextBlock::OFFSET_CONTEXT_F26,
    F28=UnitContextBlock::OFFSET_CONTEXT_F28,
    F30=UnitContextBlock::OFFSET_CONTEXT_F30
};
Q_ENUM_NS(DataBaseHeaders)


}//namespace

#endif // UNITBLOCKSDEFINITION_H
