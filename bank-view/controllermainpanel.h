#ifndef CONTROLLERMAINPANEL_H
#define CONTROLLERMAINPANEL_H

#include <QObject>

#include <modbuschannel.h>

#include <definitionsbaselayer.h>
#include <definitionunitblocks.h>
#include <definitionauxiliarykeys.h>

#include <junctionbankdatabase.h>

#include <controllerbanktransfer.h>

#include <QTimer>

//!
//! \brief The ControllerMainPanel class
//! Used to fetch out Pause/Error/Error codes
//!  Control Pause/Error Reset
//!  TODOS
//!  need data base to translate error messages

using namespace HEADER_STRUCTURE;

class ControllerMainPanel : public QObject
{
    Q_OBJECT
public:
    enum MainPanelContext
    {
        //! Operations
        ERROR_RESET_BIT=0x03000020,
        MANUAL_CONTROL_WORD=0x03000020,
        MANUAL_TOGGLE_PAUSE=0x03010020,
        MANUAL_TOGGLE_INIT=0x03020020,
        MANUAL_TOOGLE_MANUAL=0x03030020,
        ERROR_IGNORE_BIT=0x03040020,
        MANUAL_TOOGLE_CLEAR=0x03050020,
        //! Monitors
        ERROR_DEVICE_INDEX=0x03000022,
        ERROR_CATEGRORY=0x03000024,
        ERROR_CODE=0x03000026,

        MON_DATA_0 = 0x03000030,
        MON_DATA_1= 0x03000032,
        MON_DATA_2= 0x03000034,
        MON_DATA_3= 0x03000036,
        MON_DATA_4= 0x03000038,
        MON_DATA_5= 0x0300003A,
        MON_DATA_6= 0x0300003C,
        MON_DATA_7= 0x0300003E,

        ET_USAGE_THRESHOLD = MON_DATA_1,
        ET_USAGE_COUNTER = MON_DATA_2,
        ET_NG_MIND = MON_DATA_3,
        ET_NG_THRESHOLD = MON_DATA_7,

        OFFSET_CONTEXT_LUID_PARENT = 0x03000000+UnitContextBlock::OFFSET_CONTEXT_LUID_PARENT,
    };
    Q_ENUM(MainPanelContext)
    enum MainStates
    {
        STATE_AUTO,
        STATE_SEMI_AUTO,
        STATE_MANUAL,
    };
    Q_ENUM(MainStates)
    enum InitializingStates
    {
        WAIT_INITIAL = 0,
        INITIALING,
        INITIALIZED = 0x100
    };

    //! Send one shot command
    void Pause()
    {
        __channel->Access<bool>(ModbusDriverAddress(MANUAL_TOGGLE_PAUSE),true);
        emit stateChanged(CurrentState());
    }
    void Manual(bool value)
    {
        //!Reject
        if(!IsPause())
            return;

        //! Manual->SemiAuto , clear all task
        if(!value)
        {
            __controllerTransfer->Direction(CommitBlock::MODE_DOWNLOAD_DATA_BLOCK);
            __controllerTransfer->onTransferData();
            return;
        }

        //! Semi->Manaul , unconditional
        __channel->Access(ModbusDriverAddress(MANUAL_TOOGLE_MANUAL),true);
        emit stateChanged(CurrentState());
    }

    bool IsPause() const
    {
        ModbusDriverAddress __address(UnitOperationBlock::OFFSET_UOB_STATE_PAUSE);
        __address.setChannel(3);
        return __channel->Access<bool>(__address);
    }
    void Initialize()
    {
        __channel->Access<bool>(ModbusDriverAddress(MANUAL_TOGGLE_INIT),true);
    }
    //!
    //! \brief InitializingState
    //! \return
    //! Reflect current initializing state
    InitializingStates InitializingState() const
    {
        //! = Controller state on H100
        ModbusDriverAddress __address(UnitOperationBlock::OFFSET_MONITOR_STATE);
        __address.setChannel(3);
        MODBUS_U_WORD state = __channel->Access<MODBUS_U_WORD>(__address);
        if(state == 0)
            return WAIT_INITIAL;
        else if(state < INITIALIZED)
            return INITIALING;
        else
            return INITIALIZED;
    }

    //!
    //! \brief ErrorReset
    //!
    void ErrorReset()
    {
        __channel->Access<bool>(ModbusDriverAddress(ERROR_RESET_BIT),true);
    }
    //!
    //! \brief ErrorIgnore
    //! Given another error reset decision
    void ErrorIgnore()
    {
        __channel->Access<bool>(ModbusDriverAddress(ERROR_IGNORE_BIT),true);
    }

    void Clear()
    {
        __channel->Access<bool>(ModbusDriverAddress(MANUAL_TOOGLE_CLEAR),true);
    }
    bool IsClear() const
    {
        return __channel->Access<bool>(ModbusDriverAddress(MANUAL_TOOGLE_CLEAR));
    }

    MODBUS_U_QUAD ErrorCode() const
    {
        return __channel->Access<MODBUS_U_QUAD>(ModbusDriverAddress(ERROR_CODE));
    }
    MODBUS_U_WORD ErrorCategrory() const
    {
        return __channel->Access<MODBUS_U_WORD>(ModbusDriverAddress(ERROR_CATEGRORY));
    }
    MODBUS_U_WORD ErrorDeviceIndex() const
    {
        return __channel->Access<MODBUS_U_WORD>(ModbusDriverAddress(ERROR_DEVICE_INDEX));
    }

    QString ErrorDevice() const;
    QString ErrorDescription() const;

    MainStates CurrentState()
    {
        MainStates __state;
        if(!IsPause())
            __state = STATE_AUTO;
        else
        {
            if(__channel->Access<bool>(ModbusDriverAddress(MANUAL_TOOGLE_MANUAL)))
                __state = STATE_MANUAL;
            else
                __state =  STATE_SEMI_AUTO;
        }

        return __state;
    }

    void Data(uint key,QVariant value)
    {
        __channel->Access(ModbusDriverAddress(key),value);
    }
    QVariant Data(uint key) const
    {
        MODBUS_U_LONG value = __channel->Access<MODBUS_U_LONG>(ModbusDriverAddress(key));

        switch (key) {
        case MON_DATA_4:
        case MON_DATA_5:
        case MON_DATA_6:
            return QVariant::fromValue(value * AbstractDataBlock::Dimension->value(TIME));
            break;
        default:
            return QVariant::fromValue(value);
            break;
        }
    }

    const ControllerBankTransfer* ControllerTransfer() const
    {
        return __controllerTransfer;
    }

    static ControllerMainPanel* Instance();
signals:
    void stateChanged(MainStates currentState);
    void errorChanged(MODBUS_U_QUAD currentError);
    void firstTimeAck();
public slots:
    void onDataChanged(TransferTask task);
protected slots:
    void onReply();
    void onDataTransfered();
protected:
    explicit ControllerMainPanel(QObject *parent = nullptr);

    ModbusChannel* __channel;
    ModbusDriverAddress __monitorBaseAddress;

    HEADER_STRUCTURE::Headers __key;
    QSqlTableModel* __deviceTable;
    //!
    //! \brief __deviceMap
    //! Key , Device categrory
    QMap<MODBUS_S_WORD,QSqlTableModel*> __deviceMap;
    //!
    //! \brief __errorDescriptionMap
    //! Key 1: Device categrory
    //! Key 2: Error Code
    QMap<MODBUS_S_WORD,QSqlTableModel*> __errorCodeMap;

    MainStates __lastState;
    MODBUS_U_QUAD __lastError;

    ControllerBankTransfer* __controllerTransfer;

    static ControllerMainPanel* __instace;

    bool m_firstAcknowledge;
};

#endif // CONTROLLERMAINPANEL_H
