#include "frontcommonmanual.h"
#include <algorithm>

FrontCommonManual::FrontCommonManual(QWidget *parent):
    FrontCommon (parent),
    mainDataTable(nullptr),
	m_isInitialized(false)
{
	m_timer = new QTimer(this);
	connect(m_timer, &QTimer::timeout, this, &FrontCommonManual::onTimerScan);
	m_timer->start();
	//delay timer
	m_delayTimer = new QTimer(this);
	connect(m_delayTimer, &QTimer::timeout, this, &FrontCommonManual::onDelayTimesout);
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
	m_port->externalPropertyChange(QVariant::fromValue(ManualModeDataBlock::MON_DEVICE_INDEX),
		QVariant::fromValue(static_cast<MODBUS_U_WORD>(currentIndex())));
}

void FrontCommonManual::showEvent(QShowEvent* event)
{
    //! Monitor selection
    if(m_port!=nullptr)
    {
		m_port->externalPropertyChange(QVariant::fromValue(ManualModeDataBlock::MON_CATEGRORY),
			m_categrory);

        onMonitorIndexChanged();
    }
    if(mainDataTable!=nullptr)
    {
        //! Resume
        mainDataTable->setFilter(currentFilter());
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
	emit qobject_cast<PropertyPortCommon*>(m_port)->externalPropertyChange(QVariant::fromValue(ManualModeDataBlock::BATCH_PRESCHEDUALED_MODE),
		true);
   
   //! Base method
   FrontCommon::hideEvent(event);
}

void FrontCommonManual::onTimerScan()
{
	/*QVariant value =
		property(QVariant::fromValue(ManualModeDataBlock::PROP_MANUAL_STATE));

	for each (QWidget* var in m_widgetLockList)
	{
		var->setEnabled(value.value<ManualModeDataBlock::ManualState>() ==
			ManualModeDataBlock::STATE_PLC_READY);
	}*/
}

void FrontCommonManual::onPropertyChanged(QVariant key, QVariant value)
{
	FrontCommon::onPropertyChanged(key, value); //base method
	switch (key.toInt())
	{
	case ManualModeDataBlock::PROP_MODEL_UPDATED:
		if (mainDataTable != nullptr)
		{
			//! updated
			mainDataTable->select();
		}
		break;
	case ManualModeDataBlock::PROP_MANUAL_STATE:
		switch (value.value<ManualModeDataBlock::ManualState>())
		{
		case ManualModeDataBlock::STATE_RUN_ON:
		case ManualModeDataBlock::STATE_DONE_ON:
			for each (QWidget* var in m_widgetLockList)
			{
				var->setEnabled(false);
			}
			m_delayTimer->start(); //start/restart
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

void FrontCommonManual::onDelayTimesout()
{
	switch (property(QVariant::fromValue(ManualModeDataBlock::PROP_MANUAL_STATE).toString().toStdString().c_str()).value<ManualModeDataBlock::ManualState>())
	{
	case ManualModeDataBlock::STATE_PLC_READY:
	case ManualModeDataBlock::STATE_IN_AUTO:
		for each (QWidget* var in m_widgetLockList)
		{
			var->setEnabled(true);
		}
		break;
	default:
		break;
	}
}