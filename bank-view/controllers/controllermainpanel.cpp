#include "controllermainpanel.h"

ControllerMainPanel::ControllerMainPanel(quint8 clientId, quint16 baseOffset, int interval, QObject *parent) :
    ControllerBase(clientId,baseOffset,interval,parent)
{

    //! register monitor , start routine service
    m_monitor =
            new MainMonitorBlock(registerWatchList(static_cast<ADDRESS_MODE>(UnitContextBlock::OFFSET_CONTEXT_LUID_PARENT),
                                                   QVariant::fromValue(CellDataBlock())));
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
	m_monitor_propertyKeys << QVariant::fromValue(MainMonitorBlock::OFFSET_MONITOR_STATE);
}


