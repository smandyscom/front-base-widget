#ifndef UNITBLOCKSDEFINITION_H
#define UNITBLOCKSDEFINITION_H

#include<definitionbasicblocks.h>

#include<abstractsqltableadpater.h>

class UnitMonitorBlock:
        public AbstractDataBlock
{
public:
    enum OffsetMonitor
    {
        STATE=1,
        ALARM_CODE=2,
        NEXT_STATE=4,
        TRANSITION_WORD=5,
    };
    enum Transition
    {
        SERIAL_TRANSITON=0,
        TEMP_CONDITION_1=8,
        TEMP_CONDITION_2,
        TEMP_CONDITION_3,
        TEMP_CONDITION_4,
        TEMP_CONDITION_5,
        TEMP_CONDITION_6,
        TEMP_CONDITION_7,
        TEMP_CONDITION_8,
    };

    UnitMonitorBlock() {}
};
Q_DECLARE_METATYPE(UnitMonitorBlock)

class UnitOperationBlock:public UnitMonitorBlock
{
public:
    enum OffsetOperation
    {
        CONTROL_WORD=3,
    };
    enum ControlWord
    {
        EN_STEP=0,
        TRIG_STEP,
        TRIG_PAUSE,
        IS_BYPASSED,
    };
};
Q_DECLARE_METATYPE(UnitOperationBlock)

class UnitContextBlock : public UnitOperationBlock
{
public:
    enum OffsetContext
    {
        LUID_PARENT=0,
        LSID_BASE=6,
        LSID_COUNT=7,
    };

    void Value(uint key, QVariant value) Q_DECL_OVERRIDE
    {
        switch (key) {
        case LUID_PARENT:
            setData(key,value.value<MODBUS_U_LONG>());
            break;
        case LSID_BASE:
        case LSID_COUNT:
            setData(key,value.value<MODBUS_U_WORD>());
            break;
        default:
            break;
        }
    }

};
Q_DECLARE_METATYPE(UnitContextBlock)

namespace UnitBlock {
Q_NAMESPACE
enum DataBaseHeaders
{
    ID,
};
Q_ENUM_NS(DataBaseHeaders)

static AbstractSqlTableAdpater* Adaptor;

}//namespace

#endif // UNITBLOCKSDEFINITION_H
