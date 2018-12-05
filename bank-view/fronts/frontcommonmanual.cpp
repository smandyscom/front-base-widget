#include "frontcommonmanual.h"

FrontCommonManual::FrontCommonManual(QWidget *parent):
    FrontCommon (parent),
    mainDataTable(nullptr),
	m_isInitialized(false)
{
	m_timer = new QTimer(this);
	connect(m_timer, &QTimer::timeout, this, &FrontCommonManual::onTimerScan);
	m_timer->start();
}

int FrontCommonManual::currentIndex()
{
    return 0;
}
QString FrontCommonManual::currentFilter() const
{
    return nullptr;
}

void FrontCommonManual::onMonitorIndexChanged()
{
    m_controller->setProperty(QVariant::fromValue(ManualModeDataBlock::MON_DEVICE_INDEX).toString().toStdString().c_str(),
                              QVariant::fromValue(static_cast<MODBUS_U_WORD>(currentIndex())));
}

void FrontCommonManual::showEvent(QShowEvent* event)
{
    //! Monitor selection
    if(m_controller!=nullptr)
    {
        m_controller->setProperty(QVariant::fromValue(ManualModeDataBlock::MON_CATEGRORY).toString().toStdString().c_str(),
                                  m_categrory);
        onMonitorIndexChanged();
    }
    if(mainDataTable!=nullptr)
    {
        //! Resume
        mainDataTable->setFilter(currentFilter());
        mainDataTable->select();
    }
    //! Base method
    FrontCommon::showEvent(event);
}
//!
//! \brief FrontCommonManual::hideEvent
//! \param event
//! Raise transfer
void FrontCommonManual::hideEvent(QHideEvent *event)
{
	//QString str = QVariant::fromValue(ManualModeDataBlock::BATCH_PRESCHEDUALED_MODE).toString();
	//if (!m_controller->property(str.toStdString().c_str()).toBool())
	//	//! Trigger
	//	m_controller->setProperty(str.toStdString().c_str(), true);
	//else
	//	qDebug() << "ManualModeDataBlock::BATCH_PRESCHEDUALED_MODE, false";
	emit qobject_cast<PropertyPortCommon*>(m_controller)->propertyChange(QVariant::fromValue(ManualModeDataBlock::BATCH_PRESCHEDUALED_MODE),
		true);
   
   //! Base method
   FrontCommon::hideEvent(event);
}

void FrontCommonManual::onTimerScan()
{
	QVariant value =
		property(QVariant::fromValue(ManualModeDataBlock::PROP_MANUAL_STATE));

	for each (QWidget* var in m_widgetLockList)
	{
		var->setEnabled(value.value<ManualModeDataBlock::ManualState>() ==
			ManualModeDataBlock::STATE_PLC_READY);
	}
}