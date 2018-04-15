#ifndef CYLINDERBLOCKDEFINITION_H
#define CYLINDERBLOCKDEFINITION_H

#include <definitionbasicblocks.h>
#include <abstractsqltableadpater.h>

namespace CylinderBlock {


class CylinderMonitorBlock : public AbstractDataBlock
{
public:
    enum OffsetMonitor
    {
        OFFSET_MONITOR_STATUS_WORD=0,
        OFFSET_MONITOR_LAST_COMMAND=2,
        OFFSET_MONITOR_TMR_COUNT_VALUE=3,
    };
    enum StatusWord : int
    {
        MOR_WARN=0x0001,
        MOR_GROUP_A=0x0002,
        MOR_GROUP_B=0x0004,
        CTL_SUPPRESS=0x0008,
        INT_TMR_ON=0x0010,
        MOR_DONE=0x0020,
    };
    enum Operation : MODBUS_WORD
    {
        OP_NO_COMMAND=0,
        OP_COMMAND_A=1,
        OP_COMMAND_B=2,
    };
    bool Bit(StatusWord bit) const Q_DECL_OVERRIDE
    {
        return (reserved[OFFSET_MONITOR_STATUS_WORD] & bit) > 0;
    }
    QVariant Value(int index) const Q_DECL_OVERRIDE
    {
        switch (index) {
        case OFFSET_MONITOR_STATUS_WORD:
            return QVariant::fromValue(reserved[index]).value<MODBUS_WORD>();
            break;
        case OFFSET_MONITOR_LAST_COMMAND:
            return QVariant::fromValue(Operation(reserved[index]));
            break;
        case OFFSET_MONITOR_TMR_COUNT_VALUE:
            return QVariant::fromValue(reserved[index]*Time());
        default:
            break;
        }
    }

};
Q_DECLARE_METATYPE(CylinderMonitorBlock)

class CylinderOperationBlock: public CylinderMonitorBlock
{
public:
    enum OffsetOperation
    {
        OFFSET_OPERATION_COMMAND_CACHED=1,
    };

    void Value(int index,MODBUS_WORD value) Q_DECL_OVERRIDE
    {
        switch (index) {
        case OFFSET_OPERATION_COMMAND_CACHED:
            reserved[index] = value;
            break;
        default:
            break;
        }
    }
};
Q_DECLARE_METATYPE(CylinderOperationBlock)
class CylinderContext: public CylinderOperationBlock
{
public:
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

    void Value(int index, MODBUS_WORD value) Q_DECL_OVERRIDE
    {
        switch (index) {
        case OFFSET_CONTEXT_TMR_SET_VALUE:
            reserved[index] = value/Time();
            break;
        case OFFSET_CONTEXT_A_SENSOR_USED_COUNT:
        case OFFSET_CONTEXT_B_SENSOR_USED_COUNT:
        case OFFSET_CONTEXT_ACT_A_1:
        case OFFSET_CONTEXT_ACT_A_2:
        case OFFSET_CONTEXT_ACT_B_1:
        case OFFSET_CONTEXT_ACT_B_2:
        case OFFSET_CONTEXT_SENSOR_A_1:
        case OFFSET_CONTEXT_SENSOR_A_2:
        case OFFSET_CONTEXT_SENSOR_A_3:
        case OFFSET_CONTEXT_SENSOR_A_4:
        case OFFSET_CONTEXT_SENSOR_B_1:
        case OFFSET_CONTEXT_SENSOR_B_2:
        case OFFSET_CONTEXT_SENSOR_B_3:
        case OFFSET_CONTEXT_SENSOR_B_4:
            reserved[index] = value;
            break;
        default:
            CylinderOperationBlock::Value(index,value);
            break;
        }
    }
    QVariant Value(int index) const Q_DECL_OVERRIDE
    {
        switch (index) {
        case OFFSET_CONTEXT_TMR_SET_VALUE:
            return QVariant::fromValue(reserved[index]*Time());
            break;
        case OFFSET_CONTEXT_A_SENSOR_USED_COUNT:
        case OFFSET_CONTEXT_B_SENSOR_USED_COUNT:
        case OFFSET_CONTEXT_ACT_A_1:
        case OFFSET_CONTEXT_ACT_A_2:
        case OFFSET_CONTEXT_ACT_B_1:
        case OFFSET_CONTEXT_ACT_B_2:
        case OFFSET_CONTEXT_SENSOR_A_1:
        case OFFSET_CONTEXT_SENSOR_A_2:
        case OFFSET_CONTEXT_SENSOR_A_3:
        case OFFSET_CONTEXT_SENSOR_A_4:
        case OFFSET_CONTEXT_SENSOR_B_1:
        case OFFSET_CONTEXT_SENSOR_B_2:
        case OFFSET_CONTEXT_SENSOR_B_3:
        case OFFSET_CONTEXT_SENSOR_B_4:
            return QVariant::fromValue(reserved[index]);
            break;
        default:
            return CylinderOperationBlock::Value(index);
            break;
        }
    }
};
Q_DECLARE_METATYPE(CylinderContext)

enum DataBaseHeaders
{
    CYL_ID = 0xFFFF,
    //! Data
    TIMER_SET=CylinderContext::OFFSET_CONTEXT_TMR_SET_VALUE,
    A_SENSOR_USED_COUNT=CylinderContext::OFFSET_CONTEXT_A_SENSOR_USED_COUNT,
    B_SENSOR_USED_COUNT=CylinderContext::OFFSET_CONTEXT_B_SENSOR_USED_COUNT,
    ACT_A_1=CylinderContext::OFFSET_CONTEXT_ACT_A_1,
    ACT_A_2=CylinderContext::OFFSET_CONTEXT_ACT_A_2,
    ACT_B_1=CylinderContext::OFFSET_CONTEXT_ACT_B_1,
    ACT_B_2=CylinderContext::OFFSET_CONTEXT_ACT_B_2,
    SEN_A_1=CylinderContext::OFFSET_CONTEXT_SENSOR_A_1,
    SEN_A_2=CylinderContext::OFFSET_CONTEXT_SENSOR_A_2,
    SEN_A_3=CylinderContext::OFFSET_CONTEXT_SENSOR_A_3,
    SEN_A_4=CylinderContext::OFFSET_CONTEXT_SENSOR_A_4,
    SEN_B_1=CylinderContext::OFFSET_CONTEXT_SENSOR_B_1,
    SEN_B_2=CylinderContext::OFFSET_CONTEXT_SENSOR_B_2,
    SEN_B_3=CylinderContext::OFFSET_CONTEXT_SENSOR_B_3,
    SEN_B_4=CylinderContext::OFFSET_CONTEXT_SENSOR_B_4,
};
Q_ENUM_NS(DataBaseHeaders)

//!
//! \brief The CylinderSqlTableAdaptor class
//! used to calculating sensor used count
class CylinderSqlTableAdaptor : public AbstractSqlTableAdpater
{
    Q_OBJECT
public:
    CylinderSqlTableAdaptor(QObject* parent=nullptr):
        AbstractSqlTableAdpater(parent)
    {
        __concreteTypeBlock = new CylinderContext();
        __headerList = utilities::listupEnumVariant<DataBaseHeaders>();

        __headerList.removeOne(QVariant::fromValue(CylinderContext::OFFSET_CONTEXT_A_SENSOR_USED_COUNT));
        __headerList.removeOne(QVariant::fromValue(CylinderContext::OFFSET_CONTEXT_B_SENSOR_USED_COUNT));
    }

    QVariant record2Data(const QSqlRecord &record) Q_DECL_OVERRIDE
    {
       *__concreteTypeBlock =  AbstractSqlTableAdpater::record2Data(record);

        //! Calculating sensor usage
        MODBUS_WORD counter=0;
        QList<CylinderContext> __addressList;

        //! Sensor A used
        counter = 0;
        __addressList = {
                    CylinderContext::OFFSET_CONTEXT_SENSOR_A_1,
                    CylinderContext::OFFSET_CONTEXT_SENSOR_A_2,
                    CylinderContext::OFFSET_CONTEXT_SENSOR_A_3,
                    CylinderContext::OFFSET_CONTEXT_SENSOR_A_4,
        };
        foreach (CylinderContext::OffsetContext var, __addressList) {
            if(__concreteTypeBlock->Value(var).toInt() != 0)
                counter++;
        }
        __concreteTypeBlock->Value(CylinderContext::OFFSET_CONTEXT_A_SENSOR_USED_COUNT,QVariant::fromValue(counter));

        //! Sensor B used
        counter = 0;
        __addressList = {
                    CylinderContext::OFFSET_CONTEXT_SENSOR_B_1,
                    CylinderContext::OFFSET_CONTEXT_SENSOR_B_2,
                    CylinderContext::OFFSET_CONTEXT_SENSOR_B_3,
                    CylinderContext::OFFSET_CONTEXT_SENSOR_B_4,
        };
        foreach (CylinderContext::OffsetContext var, __addressList) {
            if(__concreteTypeBlock->Value(var).toInt() != 0)
                counter++;
        }
        __concreteTypeBlock->Value(CylinderContext::OFFSET_CONTEXT_B_SENSOR_USED_COUNT,QVariant::fromValue(counter));

        return QVariant::fromValue(__concreteTypeBlock);
    }
};


}//namespace

#endif // CYLINDERBLOCKDEFINITION_H
