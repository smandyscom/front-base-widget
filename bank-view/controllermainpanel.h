#ifndef CONTROLLERMAINPANEL_H
#define CONTROLLERMAINPANEL_H

#include <QObject>

#include <modbuschannel.h>
#include <definitionsbaselayer.h>
#include <definitionunitblocks.h>

#include <QTimer>

//!
//! \brief The ControllerMainPanel class
//! Used to fetch out Pause/Error/Error codes
//!  Control Pause/Error Reset
//!  TODOS
//!  need data base to translate error messages

class ControllerMainPanel : public QObject
{
    Q_OBJECT
public:
    enum MainPanelContext
    {
        ERROR_RESET_BIT=0x03000020,
        MANUAL_CONTROL_WORD=0x03000020,
        MANUAL_TOGGLE_PAUSE=0x03010020,
        MANUAL_TOGGLE_INIT=0x03020020,
        MANUAL_TOOGLE_MANUAL=0x03030020,
        ERROR_DEVICE_INDEX=0x03000022,
        ERROR_CATEGRORY=0x03000024,
        ERROR_CODE=0x03000026,

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

    //! Send one shot command
    void Pause()
    {
        __channel->Access<bool>(ModbusDriverAddress(MANUAL_TOGGLE_PAUSE),true);
    }
    void Manual(bool value)
    {
        //!Reject
        if(!IsPause())
            return;
        __channel->Access(ModbusDriverAddress(MANUAL_TOOGLE_MANUAL),value);
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
    //! \brief IsInitialize
    //! \return
    //! is in auto run routine
    bool IsInitialized() const
    {
        //! = Controller state on H100
        ModbusDriverAddress __address(UnitOperationBlock::OFFSET_MONITOR_STATE);
        __address.setChannel(3);
        return __channel->Access<MODBUS_U_WORD>(__address) == 0x100;
    }
    //!
    //! \brief ErrorReset
    //!
    void ErrorReset()
    {
        return __channel->Access<bool>(ModbusDriverAddress(ERROR_RESET_BIT),true);
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

        if(__lastState!=__state)
            emit stateChanged(__state);
        __lastState = __state;
        return __state;
    }

    static ControllerMainPanel* Instance();
signals:
    void stateChanged(MainStates currentState);
public slots:
protected slots:
    void onReply(UpdateEvent* event);
protected:
    explicit ControllerMainPanel(QObject *parent = nullptr);

    ModbusChannel* __channel;
    ModbusDriverAddress __monitorBaseAddress;

    MainStates __lastState;
    static ControllerMainPanel* __instace;
};

#endif // CONTROLLERMAINPANEL_H
