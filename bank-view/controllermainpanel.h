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
        ERROR_RESET_BIT=0x03000018,
        ERROR_CATEGRORY=0x300001A,
        ERROR_DEVICE_INDEX=0x300001B,
        ERROR_CODE=0x300001C,
        MANUAL_CONTROL_WORD=0x03000020,
        MANUAL_TOGGLE_PAUSE=0x03000020,
        MANUAL_TOGGLE_INIT=0x03010020,
    };
    Q_ENUM(MainPanelContext)


    //! Send one shot command
    void Pause()
    {
        __channel->Access<bool>(ModbusDriverAddress(MANUAL_TOGGLE_PAUSE),true);
    }
    bool IsPause() const
    {
        return __channel->Access<bool>(UnitMonitorBlock::TRIG_PAUSE);
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
        return __channel->Access<MODBUS_WORD>(ModbusDriverAddress(UnitMonitorBlock::STATE)) == 0x100;
    }
    //!
    //! \brief ErrorReset
    //!
    void ErrorReset()
    {
        return __channel->Access<bool>(ModbusDriverAddress(ERROR_RESET_BIT),true);
    }

    MODBUS_QUAD ErrorCode() const
    {
        return __channel->Access<MODBUS_QUAD>(ModbusDriverAddress(ERROR_CODE));
    }
    MODBUS_WORD ErrorCateGrory() const
    {
        return __channel->Access<MODBUS_WORD>(ModbusDriverAddress(ERROR_CATEGRORY));
    }
    MODBUS_WORD ErrorDeviceIndex() const
    {
        return __channel->Access<MODBUS_WORD>(ModbusDriverAddress(ERROR_DEVICE_INDEX));
    }

    static ControllerMainPanel* Instance();
signals:

public slots:
protected slots:
    void onReply(UpdateEvent* event);
protected:
    explicit ControllerMainPanel(QObject *parent = nullptr);

    ModbusChannel* __channel;

    static ControllerMainPanel* __instace;
};

#endif // CONTROLLERMAINPANEL_H
