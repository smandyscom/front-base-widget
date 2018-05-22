#ifndef CONTROLLERSAFETY_H
#define CONTROLLERSAFETY_H

#include <QObject>
#include <QTimer>

#include <modbuschannel.h>

class SafetyMonitorBlock
{
public:
    SafetyMonitorBlock() {}
protected:
    MODBUS_U_WORD reserved[16];
};
Q_DECLARE_METATYPE(SafetyMonitorBlock)

//!
//! \brief The ControllerSafety class
//! Do Safety related I/O control
class ControllerSafety : public QObject
{
    Q_OBJECT
public:
    enum Address
    {
        MONITOR=0x300200,
        //! Control
        DOOR_SOLENOID_CONTROL=0x000000,
        INTERLOCK_BYPASS=0x010000,
    };


    void DoorSolenoid(bool value)
    {
        __channel->Access<bool>(ModbusDriverAddress(DOOR_SOLENOID_CONTROL),value);
    }
    void InterlockBypass(bool value)
    {
        __channel->Access<bool>(ModbusDriverAddress(INTERLOCK_BYPASS),value);
    }

    bool Status(MODBUS_ADDRESS address) const
    {
        return __channel->Access<bool>(ModbusDriverAddress(address));
    }

    static ControllerSafety* Instance();

signals:
    //!
    //! \brief initialized
    //! When initial state polled from PLC
    void initialized();
protected slots:
    void onReply();
protected:
    explicit ControllerSafety(QObject *parent = nullptr);

    ModbusChannel* __channel;

    static ControllerSafety* __instance;

};

#endif // CONTROLLERSAFETY_H
