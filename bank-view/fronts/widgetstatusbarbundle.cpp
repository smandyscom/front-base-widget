#include "widgetstatusbarbundle.h"

#include <qdatetime.h>

WidgetStatusBarBundle::WidgetStatusBarBundle(QWidget *parent)
	: QWidget(parent)
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
	ui.labelCurrentTime->setText(QDateTime::currentDateTime().toString("yyyyMMddhhmmss"));
}