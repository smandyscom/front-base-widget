#pragma once

#include <QWidget>
#include "ui_frontsignal.h"

#include <frontcommonmanual.h>
#include <definitionsignalblock.h>

class FrontSignal : public FrontCommonManual
{
	Q_OBJECT

public:
	FrontSignal(QWidget *parent = Q_NULLPTR);
	~FrontSignal();

	void Setup(QSqlTableModel* slot,
		QSqlTableModel* header);

private:
	Ui::FrontSignal ui;

protected:
	SignalMonitor m_monitorBlock;

	QList<QVariant> m_status;

	int m_index;
	void dynamicPropertyChanged(int key, QVariant value) Q_DECL_OVERRIDE;
	int currentIndex() Q_DECL_OVERRIDE;
};
