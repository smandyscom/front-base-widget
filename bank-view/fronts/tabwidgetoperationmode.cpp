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
}
void TabWidgetOperationMode::Setup(QObject *controller)
{
    m_controller = controller;
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

    m_controller->setProperty(QVariant::fromValue(MainOperationBlock::BIT_3_TOGGLE_MANUAL).toString().toStdString().c_str(),
                              currentWidget()->property(QVariant::fromValue(HEADER_STRUCTURE::STATE_MANUAL).toString().toStdString().c_str()).toBool());
}
