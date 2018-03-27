#ifndef CYLINDERBLOCKDEFINITION_H
#define CYLINDERBLOCKDEFINITION_H

#include <basicblocksdefinition.h>

class CylinderMonitorBlock : AbstractDataBlock
{
public:
    enum OffsetMonitor
    {
        OFFSET_MONITOR_STATUS_WORD=0,
        OFFSET_MONITOR_LAST_COMMAND=2,
        OFFSET_MONITOR_TMR_COUNT_VALUE=3,
    };
    enum StatusWord
    {
        MOR_WARN=0x0001,
        MOR_GROUP_A=0x0002,
        MOR_GROUP_B=0x0004,
        CTL_SUPPRESS=0x0008,
        INT_TMR_ON=0x0010,
        MOR_DONE=0x0020,
    };

    bool Status(StatusWord bit) const
    {
        return (reserved[OFFSET_MONITOR_STATUS_WORD] & bit) > 0;
    }
};
Q_DECLARE_METATYPE(CylinderMonitorBlock)

class CylinderOperationBlock:AbstractDataBlock
{
public:

    enum OffsetOperation
    {
        OFFSET_OPERATION_COMMAND_CACHED=1,
    };
    enum Operation
    {
        OP_NO_COMMAND=0,
        OP_COMMAND_A=1,
        OP_COMMAND_B=2,
    };
    void CommandsOperation(Operation value)
    {
        reserved[OFFSET_OPERATION_COMMAND_CACHED] = value;
    }
};
Q_DECLARE_METATYPE(CylinderOperationBlock)
class CylinderContext:AbstractDataBlock
{
    enum OffsetContext
    {
        OFFSET_CONTEXT_TMR_SET_VALUE=16,
        OFFSET_CONTEXT_A_SENSOR_USED_COUNT=18,
        OFFSET_CONTEXT_B_SENSOR_USED_COUNT=19,
        OFFSET_CONTEXT_ACT_A_1=20,
        OFFSET_CONTEXT_ACT_A_2=21,
        OFFSET_CONTEXT_ACT_B_1=22,
        OFFSET_CONTEXT_ACT_B_2=23,
        OFFSET_CONTEXT_SENSOR_A_1=24,
        OFFSET_CONTEXT_SENSOR_A_2=25,
        OFFSET_CONTEXT_SENSOR_A_3=26,
        OFFSET_CONTEXT_SENSOR_A_4=27,
        OFFSET_CONTEXT_SENSOR_B_1=28,
        OFFSET_CONTEXT_SENSOR_B_2=29,
        OFFSET_CONTEXT_SENSOR_B_3=30,
        OFFSET_CONTEXT_SENSOR_B_4=31,
    };
public:

};
Q_DECLARE_METATYPE(CylinderContext)
#endif // CYLINDERBLOCKDEFINITION_H
