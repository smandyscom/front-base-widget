#include "tabwidgetoperationmode.h"
#include <QDebug>

TabWidgetOperationMode::TabWidgetOperationMode(QWidget *parent) :
    QTabWidget(parent),
    m_controller(nullptr)
{
    connect(this,
            &TabWidgetOperationMode::currentChanged,
            this,
            &TabWidgetOperationMode::onCurrentWidgetChanged);

	m_controller = new PropertyPortCommon(this);
}

//!
//! \brief TabWidgetOperationMode::onCurrentWidgetChanged
//! Fetch preseted property then set ManualBit
void TabWidgetOperationMode::onCurrentWidgetChanged()
{
    if(currentWidget()==nullptr ||
            m_controller==nullptr)
    {
        qDebug() << "onCurrentWidgetChanged , null";
        return;
    }

	QVariant value = currentWidget()->property(QVariant::fromValue(HEADER_STRUCTURE::STATE_MANUAL).toString().toStdString().c_str());
	QVariant key = QVariant::fromValue(MainOperationBlock::BIT_3_TOGGLE_MANUAL);

    m_controller->setProperty(key.toString().toStdString().c_str(),
		value.toBool());
}
