#include "widgetstatusbarbundle.h"

#include <qdatetime.h>
#include <qdebug.h>

WidgetStatusBarBundle::WidgetStatusBarBundle(QWidget *parent)
	: FrontCommon(parent)
{
	ui.setupUi(this);

	m_timer = new QTimer(this);
	connect(m_timer, &QTimer::timeout, this, &WidgetStatusBarBundle::onTimerScan);
	m_timer->start(1000);
	//
	for each (QWidget* var in findChildren<QLabel*>())
	{
		m_widgetsPolish << var;
	}
}

WidgetStatusBarBundle::~WidgetStatusBarBundle()
{
}

void WidgetStatusBarBundle::onTimerScan()
{
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
		ui.lcdNumberElapsedTime->display((value.toReal() / 1000));
		break;
	case MainMonitorBlock::OFFSET_MONITOR_STATE:
		ui.labelInitializingStatus->setText(QVariant::fromValue(MainMonitorBlock::InitializingStatus(value.toInt())).toString());
		break;
	case MainMonitorBlock::OFFSET_UOB_STATE_PAUSE:
		break;
	case MODEL:
		//ui.labelModel->setText(value.toString());
		ui.textBrowser->setText(value.toString());
		break;
	case ManualModeDataBlock::PROP_QUEUE_COUNT:
		ui.progressBarTaskQueue->reset();
		ui.progressBarTaskQueue->setRange(0,value.toInt());
		break;
	case ManualModeDataBlock::PROP_QUEUE_REMAIN:
		ui.progressBarTaskQueue->setValue(ui.progressBarTaskQueue->maximum()-
		value.toInt());
		break;
	default:
		break;
	}
}