#include "controllermainpanel.h"

ControllerMainPanel::ControllerMainPanel(QObject *parent) :
    ControllerBase(0,256,100,parent),
    __key(HEADER_STRUCTURE::zh_TW)
{

    //! register monitor , start routine service
    m_monitor =  new MainMonitorBlock(registerWatchList(static_cast<ADDRESS_MODE>(UnitContextBlock::OFFSET_CONTEXT_LUID_PARENT),
                                                         QVariant::fromValue(CellDataBlock())),parent);
      //!
    m_errorDeviceMap[CommitBlock::SELECTION_AXIS] = JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::WHOLE_AXIS);
    m_errorDeviceMap[CommitBlock::SELECTION_CYLINDER] = JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::WHOLE_CYLINDERS);
    m_errorDeviceMap[CommitBlock::SELECTION_UNIT] = JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::WHOLE_UNITS);

    __errorCodeMap[CommitBlock::SELECTION_AXIS] = JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::ERROR_CODE_AXIS);
    __errorCodeMap[CommitBlock::SELECTION_CYLINDER] = JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::ERROR_CODE_CYLINDER);
    __errorCodeMap[CommitBlock::SELECTION_UNIT] = JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::ERROR_CODE_UNIT);

    __deviceTable = JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::DEF_OBJECT_TYPE);

    //!
//    __controllerTransfer = new ControllerBankTransfer(this);
//    connect(__controllerTransfer,SIGNAL(dataTransfered()),this,SLOT(onDataTransfered()));
    //! sync with PLC
    __propertyKeys.append(QVariant::fromValue(UnitBlock::LUID_PARENT));
    __watchList.append((QVariant::fromValue(0)));
}


void ControllerMainPanel::onInitializing(InterfaceRequest ack)
{
    this->__isInitialized = true;

    ADDRESS_MODE __address = ack.Address();
    //! Update auto/manual status
    if (ADDRESS_REGISTER(__address) == toAddressMode(MANUAL_CONTROL_WORD))
    {

    }
}

//!
//! \brief ControllerMainPanel::event
//! \param event
//! \return
//! Handling
bool ControllerMainPanel::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::DynamicPropertyChange:
        //!intercept and handling
        break;
    default:
        break;
    }

    return ControllerBase::event(event);
}


QString ControllerMainPanel::ErrorDevice()
{
    QSqlRecord __recordIndex =
            m_errorDeviceMap[ErrorCategrory()]->record(ErrorDeviceIndex());

    QSqlRecord __recordDevice =
            utilities::getSqlTableSelectedRecord(__deviceTable,
                                                 QVariant::fromValue(ID),
                                                 QVariant::fromValue(ErrorCategrory()));

    return QString("%1,%2,%3")
            .arg(__recordDevice.value(QVariant::fromValue(__key).toString()).toString())
            .arg(__recordIndex.value(QVariant::fromValue(NAME).toString()).toString())
            .arg(__recordIndex.value(QVariant::fromValue(__key).toString()).toString());
}
QString ControllerMainPanel::ErrorDescription()
{
    MODBUS_U_QUAD __code = ErrorCode();

    if(__code==0)
        return QString("");

    QSqlTableModel* __lookupTable = __errorCodeMap[ErrorCategrory()];
    QString __description;
    for(int i=0;i<__lookupTable->rowCount();i++)
    {
        QSqlRecord __record = __lookupTable->record(i);
        MODBUS_U_QUAD __ref = __record.value(QVariant::fromValue(ID).toString()).toULongLong();
        if((__ref & __code)!=0)
            __description.append(QString("%1\n").arg(__record.value(QVariant::fromValue(__key).toString()).toString()));
    }

    return QString("%1")
            .arg(__description);
}

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


QVariant ControllerMainPanel::propertyValues(QVariant key)
{
    if(key.toInt() == MainMonitorBlock::ERROR_CODE)
        return QVariant::fromValue(ErrorDevice() + ErrorDescription());

    return ControllerBase::propertyValues(key);
}

