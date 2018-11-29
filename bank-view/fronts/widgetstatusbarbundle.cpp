#include "widgetstatusbarbundle.h"

#include <qdatetime.h>

WidgetStatusBarBundle::WidgetStatusBarBundle(QWidget *parent)
	: FrontCommon(parent)
{
	ui.setupUi(this);

	m_timer = new QTimer(this);
	connect(m_timer, &QTimer::timeout, this, &WidgetStatusBarBundle::onTimerScan);
	m_timer->start(1000);
}

WidgetStatusBarBundle::~WidgetStatusBarBundle()
{
}

void WidgetStatusBarBundle::onTimerScan()
{
	//QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
	ui.labelCurrentTime->setText(QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss"));

	setProperty("IsAllConnected", InterfaceChannel::Instance()->IsAllConnected());
	if (InterfaceChannel::Instance()->IsAllConnected())
		ui.labelLinkStatus->setText("Linked");
	else
		ui.labelLinkStatus->setText("Link Error");
}

void WidgetStatusBarBundle::dynamicPropertyChanged(int key, QVariant value)
{
	switch (key)
	{
	case ManualModeDataBlock::PROP_MANUAL_STATE:
		ui.labelCurrentMode->setText(value.toString());
		break;
	case ManualModeDataBlock::PROP_ELAPSED_TIME:
		ui.lcdNumberElaspedTime->display((value.toReal() / 1000));
		break;
	case MainMonitorBlock::OFFSET_MONITOR_STATE:
		ui.labelInitializingStatus->setText(QVariant::fromValue(MainMonitorBlock::InitializingStatus(value.toInt())).toString());
		break;
	case MainMonitorBlock::OFFSET_UOB_STATE_PAUSE:
		break;
	
	default:
		break;
	}
}