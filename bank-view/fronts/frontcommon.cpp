#include "frontcommon.h"

FrontCommon::FrontCommon(QWidget *parent) :
    QWidget(parent),
    m_controller(nullptr)
{
    //!css loading?
    onReloadQss();
    ///! https://stackoverflow.com/questions/24254006/rightclick-event-in-qt-to-open-a-context-menu
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this,&FrontCommon::customContextMenuRequested,this,&FrontCommon::onCustomContextMenuShowed);
}

void FrontCommon::onCustomContextMenuShowed(const QPoint position)
{
    QMenu contextMenu(tr("Context menu"),this);

    QAction action1("Reload QSS", this);
    connect(&action1,&QAction::triggered,this,&FrontCommon::onReloadQss);
    contextMenu.addAction(&action1);
    contextMenu.exec(mapToGlobal(position));
}

void FrontCommon::onReloadQss()
{
    CommonHelper::setStyle(QString("%1.qss").arg(objectName()),this);
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

//    if(m_controller == nullptr)
//        m_controller = findChild<ControllerBase*>();


    return QWidget::event(event);
}

void FrontCommon::LinkController(QObject* controller)
{
    m_controller = controller;
}
