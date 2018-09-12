#include "frontcommon.h"

FrontCommon::FrontCommon(ControllerBase *controller, QWidget *parent) :
    QWidget(parent),
    m_controller(controller)
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

    return QWidget::event(event);
}
