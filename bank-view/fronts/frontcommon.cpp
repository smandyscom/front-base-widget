#include "frontcommon.h"

FrontCommon::FrontCommon(QWidget *parent) :
    QWidget(parent),
    m_port(nullptr),
    m_isQSSInitialized(false)
{
    //!css loading?
//    onReloadQss();
    ///! https://stackoverflow.com/questions/24254006/rightclick-event-in-qt-to-open-a-context-menu
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this,&FrontCommon::customContextMenuRequested,this,&FrontCommon::onCustomContextMenuShowed);



#ifndef QT_NO_DEBUG
	m_watcher = new QFileSystemWatcher(this);
	connect(m_watcher,&QFileSystemWatcher::fileChanged, this, &FrontCommon::onReloadQss);

	onLinkQSSFile();
	connect(this, &FrontCommon::objectNameChanged, this, &FrontCommon::onLinkQSSFile);

#endif // DEBUG


	
	

	m_port = new PropertyPortCommon(this);
	connect(m_port, &PropertyPortCommon::internalPropertyChange, this, &FrontCommon::onPropertyChanged);

	m_updateTimer = new QTimer(this);
	connect(m_updateTimer, &QTimer::timeout, this, &FrontCommon::onUpdate);
	m_updateTimer->start(100);
}

void FrontCommon::activateBlink()
{
	//! BLINK
	m_blinkTimer = new QTimer(this);
	connect(m_blinkTimer, &QTimer::timeout, [=]() {
		setProperty(QVariant::fromValue(PROP_BLINK).toString().toStdString().c_str(),
			!property(QVariant::fromValue(PROP_BLINK).toString().toStdString().c_str()).toBool());
	});
	m_blinkTimer->start(1000);// every 1 second blink once
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
void FrontCommon::onLinkQSSFile()
{
	m_watcher->removePaths(m_watcher->files());
	m_watcher->addPath(QString("%1.qss").arg(objectName()));
}

bool FrontCommon::event(QEvent* event)
{
    switch (event->type()) {
    case QEvent::DynamicPropertyChange:
    {
        //!Polish 
        QString key = QString(static_cast<QDynamicPropertyChangeEvent*>(event)->propertyName());
        QVariant value = property(static_cast<QDynamicPropertyChangeEvent*>(event)->propertyName());
        //dynamicPropertyChanged(key,value);
        bool result = false;
        int id = key.toInt(&result);
        if(result)
            dynamicPropertyChanged(id,value);
        /*foreach (QWidget* var, m_widgetsPolish) {
            var->style()->polish(var);
        }*/
//        style()->polish(this);
        break;
    }
    case QEvent::Show:
        if(!m_isQSSInitialized)
        {
            onReloadQss();
            m_isQSSInitialized = true;
        }
        break;
    default:
        break;
    }

//    if(m_controller == nullptr)
//        m_controller = findChild<ControllerBase*>();


    return QWidget::event(event);
}

void FrontCommon::LinkController(QObject* controller)
{
    m_port = qobject_cast<PropertyPortCommon*>(controller);
}

QVariant FrontCommon::property(QVariant key) const
{
    return QWidget::property(key.toString().toStdString().c_str());
}

QObject* FrontCommon::port() const
{
	return m_port;
}

//!Turns key,value into dynamic property
void FrontCommon::onPropertyChanged(QVariant key, QVariant value)
{
	setProperty(key.toString().toStdString().c_str(), value);
	setProperty(QString::number(key.toULongLong()).toStdString().c_str(), value);
}

void FrontCommon::onUpdate()
{
	foreach(QWidget* var, m_widgetsPolish) {
		var->style()->polish(var);
	}
}

void FrontCommon::dynamicPropertyChanged(int key, QVariant value)
{
	qDebug() << QString("%1,%2").arg(key).arg(value.toString());
}