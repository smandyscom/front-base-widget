#include "frontcommon.h"

FrontCommon::FrontCommon(QWidget *parent) : QWidget(parent)
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
