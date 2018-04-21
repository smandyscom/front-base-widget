#ifndef CYLINDERBLOCKDEFINITION_H
#define CYLINDERBLOCKDEFINITION_H

#include <definitionbasicblocks.h>
#include <definitionauxiliarykeys.h>

#include <abstractsqltableadpater.h>

using namespace DEF_BASIC_DIMENSION;

class CylinderMonitorBlock : public AbstractDataBlock
{
public:
    enum OffsetMonitor
    {
        OFFSET_MONITOR_STATUS_WORD=0,
        OFFSET_MONITOR_LAST_COMMAND=2,
        OFFSET_MONITOR_TMR_COUNT_VALUE=3,
    };
    enum Status
    {
        //! Bit
        MOR_WARN=0,
        MOR_GROUP_A,
        MOR_GROUP_B,
        CTL_SUPPRESS,
        INT_TMR_ON,
        MOR_DONE,
    };

    enum Operation
    {
        OP_NO_COMMAND=0,
        OP_COMMAND_A=1,
        OP_COMMAND_B=2,
    };

    //!
    //! \brief Value
    //! \param index
    //! \return
    //! Readonly
    QVariant Value(uint key) const
    {
        switch (key) {
        case OFFSET_MONITOR_TMR_COUNT_VALUE:
            return QVariant::fromValue(getData<MODBUS_U_WORD>(key) * Dimension[TIME]);
        case OFFSET_MONITOR_LAST_COMMAND:
            return QVariant::fromValue(getData<MODBUS_U_WORD>(key));
        default:
            return QVariant::fromValue(Bit(key));
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

    //!
    //! \brief Value
    //! \param index
    //! \param value
    //! Write-only
    void Value(uint key,QVariant value)
    {
        AbstractDataBlock::Value(key,value);
    }
    QVariant Value(uint key) const
    {
        return CylinderMonitorBlock::Value(key);
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

    void Value(uint index, QVariant value)
    {
        switch (index) {
        case OFFSET_CONTEXT_TMR_SET_VALUE:
            setData(index,static_cast<MODBUS_U_WORD>(value.toReal() / Dimension[TIME]));
            break;
        default:
            CylinderOperationBlock::Value(index,value);
            break;
        }
    }
    QVariant Value(uint index) const
    {
        switch (index) {
        case OFFSET_CONTEXT_TMR_SET_VALUE:
            return QVariant::fromValue(getData<MODBUS_U_WORD>(index) * Dimension[TIME]);
            break;
        default:
            return CylinderOperationBlock::Value(index);
            break;
        }
    }
};
Q_DECLARE_METATYPE(CylinderContext)


namespace CylinderBlock {
Q_NAMESPACE
enum DataBaseHeaders
{
    ID = -1,
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

static AbstractSqlTableAdpater* Adaptor= nullptr;

}//namespace

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
        __headerList = utilities::listupEnumVariant<CylinderBlock::DataBaseHeaders>();

        __headerList.removeOne(QVariant::fromValue(CylinderBlock::A_SENSOR_USED_COUNT));
        __headerList.removeOne(QVariant::fromValue(CylinderBlock::B_SENSOR_USED_COUNT));
    }

    AbstractDataBlock record2Data(const QSqlRecord &record) Q_DECL_OVERRIDE
    {
       *__concreteTypeBlock =  AbstractSqlTableAdpater::record2Data(record);

        //! Calculating sensor usage
        MODBUS_U_WORD counter=0;
        //!Sensor A used
        counter = 0;
        QList<CylinderContext::OffsetContext> __addressAList = {
            CylinderContext::OFFSET_CONTEXT_SENSOR_A_1,
            CylinderContext::OFFSET_CONTEXT_SENSOR_A_2,
            CylinderContext::OFFSET_CONTEXT_SENSOR_A_3,
            CylinderContext::OFFSET_CONTEXT_SENSOR_A_4,
        };
        foreach (CylinderContext::OffsetContext var, __addressAList) {
            if(__concreteTypeBlock->Value(var).toInt() != 0)
                counter++;
        }
        __concreteTypeBlock->Value(CylinderContext::OFFSET_CONTEXT_A_SENSOR_USED_COUNT,QVariant::fromValue(counter));

        //! Sensor B used
        counter = 0;
        QList<CylinderContext::OffsetContext> __addressBList = {
                    CylinderContext::OFFSET_CONTEXT_SENSOR_B_1,
                    CylinderContext::OFFSET_CONTEXT_SENSOR_B_2,
                    CylinderContext::OFFSET_CONTEXT_SENSOR_B_3,
                    CylinderContext::OFFSET_CONTEXT_SENSOR_B_4,
        };
        foreach (CylinderContext::OffsetContext var, __addressBList) {
            if(__concreteTypeBlock->Value(var).toInt() != 0)
                counter++;
        }
        __concreteTypeBlock->Value(CylinderContext::OFFSET_CONTEXT_B_SENSOR_USED_COUNT,QVariant::fromValue(counter));

        return *__concreteTypeBlock;
    }
};



#endif // CYLINDERBLOCKDEFINITION_H
