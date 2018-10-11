#include "frontcommonmanual.h"

FrontCommonManual::FrontCommonManual(QWidget *parent):
    FrontCommon (parent),
    mainDataTable(nullptr)
{
	for each (QPushButton* var in findChildren<QPushButton*>())
	{
		m_widgetLockList.append(var);
	}
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
                              currentIndex());
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
   //! Trigger
   m_controller->setProperty(QVariant::fromValue(ManualModeDataBlock::BATCH_PRESCHEDUALED_MODE).toString().toStdString().c_str(),
                             true);
   //! Base method
   FrontCommon::hideEvent(event);
}

void FrontCommonManual::dynamicPropertyChanged(int key, QVariant value)
{
    switch (key) {
    case ManualModeDataBlock::PROP_MAIN_STATE:
		m_isPanelBusy = value.value<ManualModeDataBlock::ManualState>() ==
			ManualModeDataBlock::STATE_PLC_READY;
		for each (QWidget* var in m_widgetLockList)
		{
			var->setEnabled(m_isPanelBusy);
		}
    default:
		FrontCommon::dynamicPropertyChanged(key, value);
        break;
    }


}
