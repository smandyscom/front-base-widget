#pragma once

#include <QWidget>
#include <qtimer.h>

#include "ui_widgetstatusbarbundle.h"

class WidgetStatusBarBundle : public QWidget
{
	Q_OBJECT

public:
	WidgetStatusBarBundle(QWidget *parent = Q_NULLPTR);
	~WidgetStatusBarBundle();

protected slots:
	void onTimerScan();
private:
	Ui::WidgetStatusBarBundle ui;

protected:
	QTimer* m_timer;
};
