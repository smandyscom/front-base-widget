#include "controllermainpanel.h"

ControllerMainPanel::ControllerMainPanel(QObject *parent) :
    ControllerBase(0,256,100,parent)
{

    //! register monitor , start routine service
    m_monitor =
            new MainMonitorBlock(registerWatchList(static_cast<ADDRESS_MODE>(UnitContextBlock::OFFSET_CONTEXT_LUID_PARENT),
                                                   QVariant::fromValue(CellDataBlock())));
    //!
//    __controllerTransfer = new ControllerBankTransfer(this);
//    connect(__controllerTransfer,SIGNAL(dataTransfered()),this,SLOT(onDataTransfered()));
    //! sync with PLC
    m_monitor_propertyKeys << (QVariant::fromValue(UnitOperationBlock::OFFSET_UOB_STATE_PAUSE));
    foreach(QVariant var,utilities::listupEnumVariant<MainOperationBlock::OperationBits>())
    {
        m_monitor_propertyKeys.append(var);
    }
    foreach(QVariant var,utilities::listupEnumVariant<MainMonitorBlock::OffsetMainMonitor>())
    {
        m_monitor_propertyKeys.append(var);
    }
    //!
    foreach (QVariant var, utilities::listupEnumVariant<MainOperationBlock::OperationBits>()) {
        m_operator_propertyKeys[var.toString()] = var;
    }
}

//!
//! \brief ControllerMainPanel::m_operator_propertyChanged
//! \param key
//! \param value
//!
void ControllerMainPanel::m_operator_propertyChanged(QVariant key, QVariant value)
{
    if(value.toBool())
    {
        m_channel->Access(this->toAddressMode(key.value<ADDRESS_MODE>()),true);
        //! Reset after used
        setProperty(key.toString().toStdString().c_str(),false);
    }
}
