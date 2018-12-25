#ifndef CYLINDERBLOCKDEFINITION_H
#define CYLINDERBLOCKDEFINITION_H

#include <definitionbasicblocks.h>
#include <definitionauxiliarykeys.h>

#include <abstractsqltableadpater.h>

#define TC3 true

using namespace DEF_BASIC_DIMENSION;

class CylinderMonitorBlock :
        public AbstractDataBlock
{
    Q_OBJECT
public:
    CylinderMonitorBlock(QObject* parent=nullptr) :
        AbstractDataBlock(parent){}
    CylinderMonitorBlock(MODBUS_U_WORD* anchor,QObject* parent=nullptr) :
        AbstractDataBlock(anchor,parent){}

    enum OffsetMonitor
    {
        OFFSET_MONITOR_STATUS_WORD=0,
        OFFSET_MONITOR_LAST_COMMAND=2,
        OFFSET_MONITOR_TMR_COUNT_VALUE=3,
    };
    Q_ENUM(OffsetMonitor)
    enum Status
    {
        //! Bit
        MOR_WARN=0 + OFFSET_MONITOR_STATUS_WORD,
        MOR_GROUP_A=0x010000 + OFFSET_MONITOR_STATUS_WORD,
        MOR_GROUP_B=0x020000 + OFFSET_MONITOR_STATUS_WORD,
        CTL_SUPPRESS=0x030000 + OFFSET_MONITOR_STATUS_WORD,
        INT_TMR_ON=0x040000 + OFFSET_MONITOR_STATUS_WORD,
        MOR_DONE=0x050000 + OFFSET_MONITOR_STATUS_WORD,
    };
    Q_ENUM(Status)
    enum Operation
    {
        OP_NO_COMMAND=0,
        OP_COMMAND_A=1,
        OP_COMMAND_B=2,
    };
    Q_ENUM(Operation)
    //!
    //! \brief Value
    //! \param index
    //! \return
    //! Readonly
    QVariant Value(uint key) const Q_DECL_OVERRIDE
    {
        switch (key) {
        case OFFSET_MONITOR_TMR_COUNT_VALUE:
            return QVariant::fromValue(getData<MODBUS_U_WORD>(key) * Dimension->value(TIME));
        case OFFSET_MONITOR_LAST_COMMAND:
            return QVariant::fromValue(getData<MODBUS_U_WORD>(key));
        default:
            return QVariant::fromValue(Bit(key));
            break;
        }
    }
    void Value(uint key,QVariant value ) Q_DECL_OVERRIDE
    {
        AbstractDataBlock::Value(key,value);
    }
};

class CylinderOperationBlock:
        public CylinderMonitorBlock
{
    Q_OBJECT
public:
    CylinderOperationBlock(QObject* parent=nullptr) :
        CylinderMonitorBlock(parent){}
    CylinderOperationBlock(MODBUS_U_WORD* anchor,QObject* parent=nullptr) :
        CylinderMonitorBlock(anchor,parent){}

    enum OffsetOperation
    {
        OFFSET_OPERATION_COMMAND_CACHED=1,
    };
    Q_ENUM(OffsetOperation)
    //!
    //! \brief Value
    //! \param index
    //! \param value
    //! Write-only
    void Value(uint key,QVariant value) Q_DECL_OVERRIDE
    {
        AbstractDataBlock::Value(key,value);
    }
    QVariant Value(uint key) const Q_DECL_OVERRIDE
    {
        return CylinderMonitorBlock::Value(key);
    }
};

class CylinderContext:
        public CylinderOperationBlock
{
    Q_OBJECT
public:
    CylinderContext(QObject* parent=nullptr) :
        CylinderOperationBlock(parent){}
    CylinderContext(MODBUS_U_WORD* anchor,QObject *parent=nullptr) :
        CylinderOperationBlock(anchor,parent){}

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
    Q_ENUM(OffsetContext)

    void Value(uint index, QVariant value) Q_DECL_OVERRIDE
    {
        switch (index) {
        case OFFSET_CONTEXT_TMR_SET_VALUE:
            setData(index,static_cast<MODBUS_U_WORD>(value.toReal() / Dimension->value(TIME)));
            break;
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
            setData(index,value.value<MODBUS_U_WORD>());
            break;
        default:
            CylinderOperationBlock::Value(index,value);
            break;
        }
    }
    QVariant Value(uint index) const Q_DECL_OVERRIDE
    {
        switch (index) {
        case OFFSET_CONTEXT_TMR_SET_VALUE:
            return QVariant::fromValue(getData<MODBUS_U_WORD>(index) * Dimension->value(TIME));
            break;
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
            return QVariant::fromValue(getData<MODBUS_U_WORD>(index));
            break;
        default:
            return CylinderOperationBlock::Value(index);
            break;
        }
    }
};

namespace CylinderBlock {
Q_NAMESPACE
enum DataBaseHeaders
{
    ID = -1,
    //! NAME
    ACT_A_1_N = -2,
    ACT_A_2_N = -3,
    ACT_B_1_N = -4,
    ACT_B_2_N = -5,
    SEN_A_1_N = -6,
    SEN_A_2_N = -7,
    SEN_A_3_N = -8,
    SEN_A_4_N = -9,
    SEN_B_1_N = -10,
    SEN_B_2_N = -11,
    SEN_B_3_N = -12,
    SEN_B_4_N = -13,
    //! HAL ADDRESS
    ACT_A_1_H = -14,
    ACT_A_2_H = -15,
    ACT_B_1_H = -16,
    ACT_B_2_H = -17,
    SEN_A_1_H = -18,
    SEN_A_2_H = -19,
    SEN_A_3_H = -20,
    SEN_A_4_H = -21,
    SEN_B_1_H = -22,
    SEN_B_2_H =  -23,
    SEN_B_3_H = -24,
    SEN_B_4_H = -25,
    //! Data
    TIMER_SET=CylinderContext::OFFSET_CONTEXT_TMR_SET_VALUE,
    A_SENSOR_USED_COUNT=CylinderContext::OFFSET_CONTEXT_A_SENSOR_USED_COUNT,
    B_SENSOR_USED_COUNT=CylinderContext::OFFSET_CONTEXT_B_SENSOR_USED_COUNT,
#ifdef TC3
	ACT_A_1 = -26,
	ACT_A_2 = -27,
	ACT_B_1 = -28,
	ACT_B_2 = -29,
	SEN_A_1 = -30,
	SEN_A_2 = -31,
	SEN_A_3 = -32,
	SEN_A_4 = -33,
	SEN_B_1 = -34,
	SEN_B_2 = -35,
	SEN_B_3 = -36,
	SEN_B_4 = -37,
#endif
#ifdef MIII
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
#endif
};
Q_ENUM_NS(DataBaseHeaders)


}//namespace

//!
//! \brief The CylinderSqlTableAdaptor class
//! used to calculating sensor used count
class CylinderSqlTableAdaptor :
        public AbstractSqlTableAdpater
{
    Q_OBJECT
public:
    CylinderSqlTableAdaptor(QSqlTableModel* parent=nullptr):
        AbstractSqlTableAdpater(parent,
                                utilities::listupEnumVariant<CylinderBlock::DataBaseHeaders>(),
                                &m_block)
    {
        m_headerList.removeOne(QVariant::fromValue(CylinderBlock::A_SENSOR_USED_COUNT));
        m_headerList.removeOne(QVariant::fromValue(CylinderBlock::B_SENSOR_USED_COUNT));
    }

    AbstractDataBlock& record2Data(const QSqlRecord &record) Q_DECL_OVERRIDE
    {
       *m_concreteBlock = *reinterpret_cast<CellDataBlock*>(AbstractSqlTableAdpater::record2Data(record).Anchor());

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
            if(m_concreteBlock->Value(var).toInt() != 0)
                counter++;
        }
        m_concreteBlock->Value(CylinderContext::OFFSET_CONTEXT_A_SENSOR_USED_COUNT,QVariant::fromValue(counter));

        //! Sensor B used
        counter = 0;
        QList<CylinderContext::OffsetContext> __addressBList = {
                    CylinderContext::OFFSET_CONTEXT_SENSOR_B_1,
                    CylinderContext::OFFSET_CONTEXT_SENSOR_B_2,
                    CylinderContext::OFFSET_CONTEXT_SENSOR_B_3,
                    CylinderContext::OFFSET_CONTEXT_SENSOR_B_4,
        };
        foreach (CylinderContext::OffsetContext var, __addressBList) {
            if(m_concreteBlock->Value(var).toInt() != 0)
                counter++;
        }
        m_concreteBlock->Value(CylinderContext::OFFSET_CONTEXT_B_SENSOR_USED_COUNT,QVariant::fromValue(counter));

        return *m_concreteBlock;
    }

protected:
    CylinderContext m_block;
};



#endif // CYLINDERBLOCKDEFINITION_H
