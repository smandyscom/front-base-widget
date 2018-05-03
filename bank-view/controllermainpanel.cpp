#include "controllermainpanel.h"

ControllerMainPanel::ControllerMainPanel(QObject *parent) : QObject(parent)
{
    __channel = ModbusChannel::Instance();

    //! Very first shot
    __monitorBaseAddress.setRegisterAddress(UnitContextBlock::OFFSET_CONTEXT_LUID_PARENT);
    __monitorBaseAddress.setChannel(3);
    __channel->beginAccess<AbstractDataBlock>(__monitorBaseAddress);

    //! Link
    connect(__channel,&ModbusChannel::raiseUpdateEvent,this,&ControllerMainPanel::onReply);
    //!
    __deviceMap[CommitBlock::SELECTION_AXIS] = JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::WHOLE_AXIS);
    __deviceMap[CommitBlock::SELECTION_CYLINDER] = JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::WHOLE_CYLINDERS);
    __deviceMap[CommitBlock::SELECTION_UNIT] = JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::WHOLE_UNITS);

    __errorCodeMap[CommitBlock::SELECTION_AXIS] = JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::ERROR_CODE_AXIS);
    __errorCodeMap[CommitBlock::SELECTION_CYLINDER] = JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::ERROR_CODE_CYLINDER);
    __errorCodeMap[CommitBlock::SELECTION_UNIT] = JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::ERROR_CODE_UNIT);

    __deviceTable = JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::DEF_OBJECT_TYPE);
    //!
    __key = zh_TW;
    //!
    __controllerTransfer = new ControllerBankTransfer(this);
    connect(__controllerTransfer,SIGNAL(dataTransfered()),this,SLOT(onDataTransfered()));
}

//!
//! \brief ControllerMainPanel::onReply
//! \param event
//!
void ControllerMainPanel::onReply(UpdateEvent *event)
{
    switch (event->address) {
    case OFFSET_CONTEXT_LUID_PARENT:
        QTimer::singleShot(100,this,[=](){
            //! Schedualing next polling
            __channel->beginAccess<AbstractDataBlock>(__monitorBaseAddress);
        });
        //!
        //! Error detection
        //!
        if((ErrorCode()!=0) != __lastError)
            emit errorChanged((ErrorCode()!=0));
        __lastError = (ErrorCode()!=0);

        break;
    default:
        break;
    }
}

ControllerMainPanel* ControllerMainPanel::Instance()
{
    if(__instace ==nullptr)
        __instace = new ControllerMainPanel();
    return __instace;
}

QString ControllerMainPanel::ErrorDevice() const
{
    QSqlRecord __recordIndex =
            __deviceMap[ErrorCategrory()]->record(ErrorDeviceIndex());

    QSqlRecord __recordDevice =
            utilities::getSqlTableSelectedRecord(__deviceTable,
                                                 QVariant::fromValue(ID),
                                                 QVariant::fromValue(ErrorCategrory()));

    return QString("%1,%2,%3")
            .arg(__recordDevice.value(QVariant::fromValue(__key).toString()).toString())
            .arg(__recordIndex.value(QVariant::fromValue(NAME).toString()).toString())
            .arg(__recordIndex.value(QVariant::fromValue(__key).toString()).toString());
}
QString ControllerMainPanel::ErrorDescription() const
{
    MODBUS_U_QUAD __code = ErrorCode();
    MODBUS_U_LONG __lower = (reinterpret_cast<MODBUS_U_LONG*>(&__code))[0];
    MODBUS_U_LONG __higher = (reinterpret_cast<MODBUS_U_LONG*>(&__code))[1];

    QString __lowerDescription =
            utilities::getSqlTableSelectedRecord(__errorCodeMap[ErrorCategrory()],
            QVariant::fromValue(ID),
            QVariant::fromValue(__lower))
            .value(QVariant::fromValue(__key).toString()).toString();
    QString __higherDescription =
            utilities::getSqlTableSelectedRecord(__errorCodeMap[ErrorCategrory()],
            QVariant::fromValue(ID),
            QVariant::fromValue(__higher))
            .value(QVariant::fromValue(__key).toString()).toString();

    return QString("%1\n%2")
            .arg(__lowerDescription)
            .arg(__higherDescription);
}

void ControllerMainPanel::onDataChanged(TransferTask task)
{
    __controllerTransfer->PutTask(task);
}

void ControllerMainPanel::onDataTransfered()
{
    //! Turns into auto mode
    __channel->Access(ModbusDriverAddress(MANUAL_TOOGLE_MANUAL),false);
}

ControllerMainPanel* ControllerMainPanel::__instace = nullptr;
