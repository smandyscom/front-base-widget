#pragma once

#include <QWidget>
#include <qtimer.h>

#include "ui_widgetstatusbarbundle.h"

#include <frontcommon.h>

#include <definitionmanualblock.h>
#include <definitionmaincontrolblock.h>
#include <interfacechannel.h>

class WidgetStatusBarBundle : 
	public FrontCommon
{
	Q_OBJECT

public:
	enum Properties
	{
		MODEL = 0x1eff00,
	};
	Q_ENUM(Properties)

	WidgetStatusBarBundle(QWidget *parent = Q_NULLPTR);
	~WidgetStatusBarBundle();

protected slots:
	void onTimerScan();
private:
	Ui::WidgetStatusBarBundle ui;

protected:
	QTimer* m_timer;

	void dynamicPropertyChanged(int key, QVariant value) Q_DECL_OVERRIDE;
};
