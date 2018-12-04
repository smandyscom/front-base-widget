#include "propertyportcommon.h"

PropertyPortCommon::PropertyPortCommon(QObject *parent)
	: QObject(parent)
{
}

PropertyPortCommon::~PropertyPortCommon()
{
}

void PropertyPortCommon::onPropertyChanged(QVariant key, QVariant value)
{
	parent()->setProperty(key.toString().toStdString().c_str(), value);
}

bool PropertyPortCommon::event(QEvent* event)
{
	switch (event->type())
	{
	case QEvent::DynamicPropertyChange:
		QDynamicPropertyChangeEvent* e = static_cast<QDynamicMetaObjectData*>(event);
		QVariant key = QVariant::fromValue(QString(e->propertyName()));
		QVariant value = property(e->propertyName());
		emit propertyChange(key, value);
		break;
	default:
		break;
	}

	return QObject::event(event);
}