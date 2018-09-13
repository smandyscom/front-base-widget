#include "controllermainpanel.h"

ControllerMainPanel::ControllerMainPanel(QObject *parent) :
    ControllerBase(0,256,100,parent),
    locale(HEADER_STRUCTURE::zh_TW)
{

    //! register monitor , start routine service
    m_monitor =  new MainMonitorBlock(registerWatchList(static_cast<ADDRESS_MODE>(UnitContextBlock::OFFSET_CONTEXT_LUID_PARENT),
                                                         QVariant::fromValue(CellDataBlock())),parent);
      //!
    m_errorDeviceMap[CommitBlock::SELECTION_AXIS] = JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::WHOLE_AXIS);
    m_errorDeviceMap[CommitBlock::SELECTION_CYLINDER] = JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::WHOLE_CYLINDERS);
    m_errorDeviceMap[CommitBlock::SELECTION_UNIT] = JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::WHOLE_UNITS);

    m_errorCodeMap[CommitBlock::SELECTION_AXIS] = JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::ERROR_CODE_AXIS);
    m_errorCodeMap[CommitBlock::SELECTION_CYLINDER] = JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::ERROR_CODE_CYLINDER);
    m_errorCodeMap[CommitBlock::SELECTION_UNIT] = JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::ERROR_CODE_UNIT);

    m_deviceTable = JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::DEF_OBJECT_TYPE);

    //!
//    __controllerTransfer = new ControllerBankTransfer(this);
//    connect(__controllerTransfer,SIGNAL(dataTransfered()),this,SLOT(onDataTransfered()));
    //! sync with PLC
    m_monitor_propertyKeys
            << QVariant::fromValue(UnitOperationBlock::OFFSET_UOB_STATE_PAUSE)
            << QVariant::fromValue(MainOperationBlock::BIT_3_TOGGLE_MANUAL)
            << QVariant::fromValue(MainOperationBlock::BIT_5_TOGGLE_CLEAR)
            << QVariant::fromValue(MainMonitorBlock::ERROR_CODE);
    //!
    foreach (QVariant var, utilities::listupEnumVariant<MainOperationBlock::OperationBits>()) {
        m_operator_propertyKeys[var.toString()] = var;
    }
}


QString ControllerMainPanel::errorDevice()
{
    auto deviceCategrory = static_cast<MainMonitorBlock*>(m_monitor)->Value(MainMonitorBlock::ERROR_CATEGRORY).toInt();
    auto deviceIndex = static_cast<MainMonitorBlock*>(m_monitor)->Value(MainMonitorBlock::ERROR_DEVICE_INDEX).toInt();

    QSqlRecord recordIndex =
            m_errorDeviceMap[deviceCategrory]->record(deviceIndex);

    QSqlRecord recordDevice =
            utilities::getSqlTableSelectedRecord(m_deviceTable,
                                                 QVariant::fromValue(ID),
                                                 QVariant::fromValue(deviceCategrory));

    return QString("%1,%2,%3")
            .arg(recordDevice.value(QVariant::fromValue(locale).toString()).toString())
            .arg(recordIndex.value(QVariant::fromValue(NAME).toString()).toString())
            .arg(recordIndex.value(QVariant::fromValue(locale).toString()).toString());
}
QString ControllerMainPanel::errorDescription()
{
    auto errorCode = static_cast<MainMonitorBlock*>(m_monitor)->Value(MainMonitorBlock::ERROR_CODE).value<MODBUS_U_QUAD>();
    auto deviceCategrory = static_cast<MainMonitorBlock*>(m_monitor)->Value(MainMonitorBlock::ERROR_CATEGRORY).toInt();

    if(errorCode==0)
        return QString("");

    QSqlTableModel* __lookupTable = m_errorCodeMap[deviceCategrory];
    QString __description;
    for(int i=0;i<__lookupTable->rowCount();i++)
    {
        QSqlRecord __record = __lookupTable->record(i);
        MODBUS_U_QUAD __ref = __record.value(QVariant::fromValue(ID).toString()).toULongLong();
        if((__ref & errorCode)!=0)
            __description.append(QString("%1\n").arg(__record.value(QVariant::fromValue(locale).toString()).toString()));
    }

    return QString("%1")
            .arg(__description);
}

//MainStates ControllerMainPanel::mainState()
//{
//    if(!static_cast<UnitOperationBlock*>(m_monitor)->Value(UnitOperationBlock::OFFSET_UOB_STATE_PAUSE).toBool())
//        return STATE_AUTO;
//    else if(static_cast<MainOperationBlock*>(m_monitor)->Value(MainOperationBlock::BIT_3_TOGGLE_MANUAL).toBool())
//        return STATE_MANUAL;
//    else
//        return STATE_SEMI_AUTO;
//}

//void ControllerMainPanel::onDataChanged(TransferTask task)
//{
//    __controllerTransfer->PutTask(task);
//}

//void ControllerMainPanel::onDataTransfered()
//{
//    //! Turns into auto mode
//    __channel->Access(ModbusDriverAddress(MANUAL_TOOGLE_MANUAL),false);
//    emit stateChanged(CurrentState());
//}

//!
//! \brief ControllerMainPanel::propertyValues
//! \param key
//! \return
//! Post infos to front
QVariant ControllerMainPanel::m_monitor_propertyValues(QVariant key)
{
    if(key == QVariant::fromValue(MainMonitorBlock::ERROR_CODE))
        return QVariant::fromValue(errorDevice() + errorDescription());

    return ControllerBase::m_monitor_propertyValues(key);
}
//!
//! \brief ControllerMainPanel::m_operator_propertyChanged
//! \param key
//! \param value
//!
void ControllerMainPanel::m_operator_propertyChanged(QVariant key, QVariant value)
{
    m_channel->Access(this->toAddressMode(key.value<ADDRESS_MODE>()),value.toBool());
}
