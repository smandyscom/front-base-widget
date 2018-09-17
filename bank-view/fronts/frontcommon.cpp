#include "frontcommon.h"

FrontCommon::FrontCommon(QWidget *parent) :
    QWidget(parent),
    m_controller(nullptr)
{
}


bool FrontCommon::event(QEvent* event)
{
    switch (event->type()) {
    case QEvent::DynamicPropertyChange:
    {
        //!Polish 
        QVariant value = property(static_cast<QDynamicPropertyChangeEvent*>(event)->propertyName());
        dynamicPropertyChanged(QString(static_cast<QDynamicPropertyChangeEvent*>(event)->propertyName()),value);
        bool result = false;
        int id = QString(static_cast<QDynamicPropertyChangeEvent*>(event)->propertyName()).toInt(&result);
        dynamicPropertyChanged(id,value);
        style()->polish(this);
        break;
    }
    default:
        break;
    }

    if(m_controller == nullptr)
        m_controller = findChild<ControllerBase*>();


    return QWidget::event(event);
}

void FrontCommon::LinkController(QObject* controller)
{
    m_controller = controller;
}
