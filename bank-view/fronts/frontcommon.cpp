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
        //!Polish
        style()->polish(this);
        break;
    default:
        break;
    }

    if(m_controller == nullptr)
        m_controller = findChild<ControllerBase*>();


    return QWidget::event(event);
}
