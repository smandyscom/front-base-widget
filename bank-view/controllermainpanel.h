#ifndef CONTROLLERMAINPANEL_H
#define CONTROLLERMAINPANEL_H

#include <QObject>
#include <QVariant>
#include <QMetaEnum>

#include <definitionsbaselayer.h>
#include <definitionunitblocks.h>
#include <definitionauxiliarykeys.h>
#include <definitionmaincontrolblock.h>

#include <junctionbankdatabase.h>

//#include <controllerbanktransfer.h>

#include <controllerbase.h>

#include <QTimer>

//!
//! \brief The ControllerMainPanel class
//! Used to fetch out Pause/Error/Error codes
//!  Control Pause/Error Reset
//!  TODOS
//!  need data base to translate error messages

using namespace HEADER_STRUCTURE;

class ControllerMainPanel :
        public ControllerBase
{
    Q_OBJECT
public:
    enum ExtendedProperties
    {
        PROP_MAIN_STATE,
    };

//    enum MainStates
//    {
//        STATE_AUTO,
//        STATE_SEMI_AUTO,
//        STATE_MANUAL,
//    };
//    Q_ENUM(MainStates)
    Q_ENUM(ExtendedProperties)

    //! Send one shot command
    void Pause()
    {
//        __channel->Access<bool>(ModbusDriverAddress(MANUAL_TOGGLE_PAUSE),true);
    }
    void Manual(bool value)
    {
//        //!Reject
//        if(!IsPause())
//            return;

//        //! Manual->SemiAuto , clear all task
//        if(!value)
//        {
//            __controllerTransfer->Direction(CommitBlock::MODE_DOWNLOAD_DATA_BLOCK);
//            __controllerTransfer->onTransferData();
//            return;
//        }

//        //! Semi->Manaul , unconditional
//        __channel->Access(ModbusDriverAddress(MANUAL_TOOGLE_MANUAL),true);
    }

//    void Initialize()
//    {
////        __channel->Access<bool>(ModbusDriverAddress(MANUAL_TOGGLE_INIT),true);
//    }
//    //!
//    //! \brief IsInitialize
//    //! \return
//    //! is in auto run routine
//    bool IsNotInitializing()
//    {
//        //! = Controller state on H100
//        ADDRESS_MODE __address = toAddressMode(UnitOperationBlock::OFFSET_MONITOR_STATE);
//        return m_channel->Access<MODBUS_U_WORD>(__address) == 0x100 ||
//                m_channel->Access<MODBUS_S_WORD>(__address) == 0;
//    }
//    //!
//    //! \brief ErrorReset
//    //!
//    void ErrorReset()
//    {
//        m_channel->Access<bool>(toAddressMode(ERROR_RESET_BIT),true);
//    }
//    //!
//    //! \brief ErrorIgnore
//    //! Given another error reset decision
//    void ErrorIgnore()
//    {
//        m_channel->Access<bool>(toAddressMode(ERROR_IGNORE_BIT),true);
//    }

//    void Clear()
//    {
//        m_channel->Access<bool>(toAddressMode(MANUAL_TOOGLE_CLEAR),true);
//    }


//    QVariant Data(uint key)
//    {
//        MODBUS_U_LONG value = m_channel->Access<MODBUS_U_LONG>(toAddressMode(key));

//        switch (key) {
//        case MON_DATA_4:
//        case MON_DATA_5:
//        case MON_DATA_6:
//        case MON_DATA_7:
//            return QVariant::fromValue(value * AbstractDataBlock::Dimension->value(TIME));
//            break;
//        default:
//            return QVariant::fromValue(value);
//            break;
//        }
//    }

//    const ControllerBankTransfer* ControllerTransfer() const
//    {
//        return __controllerTransfer;
//    }



    explicit ControllerMainPanel(QObject *parent = nullptr);
public slots:
//    void onDataChanged(TransferTask task);
protected:

//    MainStates mainState();
    QString errorDevice() ;
    QString errorDescription() ;

    HEADER_STRUCTURE::Headers locale;
    QSqlTableModel* m_deviceTable;
    //!
    //! \brief __deviceMap
    //! Key , Device categrory
    QMap<MODBUS_S_WORD,QSqlTableModel*> m_errorDeviceMap;
    //!
    //! \brief __errorDescriptionMap
    //! Key 1: Device categrory
    //! Key 2: Error Code
    QMap<MODBUS_S_WORD,QSqlTableModel*> m_errorCodeMap;


    QVariant m_monitor_propertyValues(QVariant key) Q_DECL_OVERRIDE;
    void m_operator_propertyChanged(QVariant key,QVariant value) Q_DECL_OVERRIDE;
};

#endif // CONTROLLERMAINPANEL_H
