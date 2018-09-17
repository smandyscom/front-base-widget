#ifndef DEFINITIONMAINCONTROLBLOCK_H
#define DEFINITIONMAINCONTROLBLOCK_H

#include<definitionunitblocks.h>

class MainMonitorBlock:
        public UnitOperationBlock
{
    Q_OBJECT
public:
    MainMonitorBlock(QObject* parent=nullptr) :
        UnitOperationBlock(parent)
    {

    }
    MainMonitorBlock(MODBUS_U_WORD* anchor,QObject* parent=nullptr) :
        UnitOperationBlock(anchor,parent)
    {

    }

    enum OffsetMainMonitor
    {
        //! Monitors
        ERROR_DEVICE_INDEX=0x00000022,
        ERROR_CATEGRORY=0x00000024,
        ERROR_CODE=0x00000026,

//        MAIN_STATE = 0x00000030,
//        MON_DATA_1= 0x00000032,
//        MON_DATA_2= 0x00000034,
//        MON_DATA_3= 0x00000036,
//        MON_DATA_4= 0x00000038,
//        MON_DATA_5= 0x0000003A,
//        MON_DATA_6= 0x0000003C,
//        MON_DATA_7= 0x0000003E,
    };
    Q_ENUM(OffsetMainMonitor)

    //!
    //! \brief Value
    //! \param key
    //! \return
    //! Reader
    QVariant Value(uint key) const Q_DECL_OVERRIDE
    {
        switch (key) {
        case ERROR_DEVICE_INDEX:
        case ERROR_CATEGRORY:
            return QVariant::fromValue(getData<MODBUS_U_WORD>(key));
            break;
        case ERROR_CODE:
            return QVariant::fromValue(getData<MODBUS_U_QUAD>(key));
            break;
        default:
            return UnitOperationBlock::Value(key);
            break;
        }
    }

    void Value(uint key,QVariant value) Q_DECL_OVERRIDE
    {
        UnitOperationBlock::Value(key,value);
    }
};
//Q_DECLARE_METATYPE(MainControlBlock)

class MainOperationBlock:
        public MainMonitorBlock
{
    Q_OBJECT
public:
    enum OffsetMainOperation
    {
        MANUAL_CONTROL_WORD=0x00000020,
    };
    Q_ENUM(OffsetMainOperation)
    enum OperationBits
    {
        BIT_0_ERROR_RESET = 0x000000 + MANUAL_CONTROL_WORD,
        BIT_1_TOGGLE_PAUSE = 0x010000 + MANUAL_CONTROL_WORD,
        BIT_2_TOGGLE_INIT = 0x020000 + MANUAL_CONTROL_WORD,
        BIT_3_TOGGLE_MANUAL = 0x030000 + MANUAL_CONTROL_WORD,
        BIT_4_ERROR_IGNORE = 0x040000 + MANUAL_CONTROL_WORD,
        BIT_5_TOGGLE_CLEAR = 0x050000 + MANUAL_CONTROL_WORD
    };
    Q_ENUM(OperationBits)

    QVariant Value(uint key) const Q_DECL_OVERRIDE
    {
        return MainMonitorBlock::Value(key);
    }
    void Value(uint key,QVariant value) Q_DECL_OVERRIDE
    {
        Bit(key,value.toBool());
    }
};
//Q_DECLARE_METATYPE(MainOperationBlock)

#endif // DEFINITIONMAINCONTROLBLOCK_H
