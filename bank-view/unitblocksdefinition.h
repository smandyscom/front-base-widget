#ifndef UNITBLOCKSDEFINITION_H
#define UNITBLOCKSDEFINITION_H

#include<basicblocksdefinition.h>

class UnitMonitorBlock:public AbstractDataBlock
{
public:
    enum OffsetMonitor
    {
        LUID_PARENT=0x03000000,
        STATE=0x03000001,
        ALARM_CODE=0x03000002,
        EN_STEP=0x03000003,
        TRIG_STEP=0x03010003,
        TRIG_PAUSE=0x03020003,
        IS_BYPASSED=0x03030003,
        TRIG_STEP_LAST=0x03040003,
        NEXT_STATE=0x03000004,
        TRANSITION_WORD=0x03000005,
        SERIAL_TRANSITON=0x03000005,
        TEMP_CONDITION_1=0x03080005,
        TEMP_CONDITION_2=0x03090005,
        TEMP_CONDITION_3=0x30A0005,
        TEMP_CONDITION_4=0x30B0005,
        TEMP_CONDITION_5=0x30C0005,
        TEMP_CONDITION_6=0x30D0005,
        TEMP_CONDITION_7=0x03000000,
        TEMP_CONDITION_8=0x30F0005,
        LSID_BASE=0x03000006,
        LSID_COUNT=0x03000007,
    };
    //Q_ENUM(OffsetMonitor)

    UnitMonitorBlock() {}
};
Q_DECLARE_METATYPE(UnitMonitorBlock)

class UnitOperationBlock:public UnitMonitorBlock
{
public:
};
Q_DECLARE_METATYPE(UnitOperationBlock)

class UnitContextBlock : public UnitOperationBlock
{
public:
};
Q_DECLARE_METATYPE(UnitContextBlock)

#endif // UNITBLOCKSDEFINITION_H
