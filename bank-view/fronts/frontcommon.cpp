#include "frontcommon.h"

FrontCommon::FrontCommon(QWidget *parent) :
    QWidget(parent),
    m_controller(nullptr)
{
    //!TODO , css loading?
}


bool FrontCommon::event(QEvent* event)
{
    switch (event->type()) {
    case QEvent::DynamicPropertyChange:
    {
        //!Polish 
        QString key = QString(static_cast<QDynamicPropertyChangeEvent*>(event)->propertyName());
        QVariant value = property(static_cast<QDynamicPropertyChangeEvent*>(event)->propertyName());
        dynamicPropertyChanged(key,value);
        bool result = false;
        int id = key.toInt(&result);
        if(result)
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
