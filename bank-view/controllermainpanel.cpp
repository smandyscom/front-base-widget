#include "controllermainpanel.h"

ControllerMainPanel::ControllerMainPanel(QObject *parent) :
    ControllerBase(0,256,100,parent),
    __key(HEADER_STRUCTURE::zh_TW)
{

    //! register monitor , start routine service
    __monitorBlock = reinterpret_cast<CellDataBlock*>(registerWatchList(static_cast<ADDRESS_MODE>(UnitContextBlock::OFFSET_CONTEXT_LUID_PARENT),QVariant::fromValue(CellDataBlock())));
//    registerWatchList(static_cast<ADDRESS_MODE>(ERROR_DEVICE_INDEX),QVariant::fromValue(AbstractDataBlock()));

      //!
    __errorDeviceMap[CommitBlock::SELECTION_AXIS] = JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::WHOLE_AXIS);
    __errorDeviceMap[CommitBlock::SELECTION_CYLINDER] = JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::WHOLE_CYLINDERS);
    __errorDeviceMap[CommitBlock::SELECTION_UNIT] = JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::WHOLE_UNITS);

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

//!
//! \brief ControllerMainPanel::onAcknowledged
//! \param ack
//! Routine
void ControllerMainPanel::onAcknowledged(InterfaceRequest ack)
{
    //!
    //! Error detection
    //!
//    if(ErrorCode() != __lastError)
//        emit errorChanged(ErrorCode());
//    __lastError = ErrorCode();

    ControllerBase::onAcknowledged(ack);
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

//ControllerMainPanel* ControllerMainPanel::Instance()
//{
//    if(__instace ==nullptr)
//        __instace = new ControllerMainPanel();
//    return __instace;
//}

QString ControllerMainPanel::ErrorDevice()
{
    QSqlRecord __recordIndex =
            __errorDeviceMap[ErrorCategrory()]->record(ErrorDeviceIndex());

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
//    switch(key.toInt())
//    {
//    case UnitBlock::LUID_PARENT:
//        return QVariant::fromValue(__channel->Access<MODBUS_U_WORD>(toAddressMode(UnitBlock::LUID_PARENT)));
//        break;
//    default:
//        return ControllerBase::propertyValues(key);
//    }
    *static_cast<CellDataBlock*>(&__monitor) = *__monitorBlock; //correct casting, value copy
    return __monitor.Value(key.toUInt());
}

//ControllerMainPanel* ControllerMainPanel::__instace = nullptr;
