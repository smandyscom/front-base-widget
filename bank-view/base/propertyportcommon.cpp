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
	{
		/*QDynamicPropertyChangeEvent* e = static_cast<QDynamicPropertyChangeEvent*>(event);
		QVariant key = QVariant::fromValue(QString(e->propertyName()));
		QVariant value = property(e->propertyName());
		emit propertyChange(key, value);*/

		QString key = QString(static_cast<QDynamicPropertyChangeEvent*>(event)->propertyName());
		QVariant value = property(static_cast<QDynamicPropertyChangeEvent*>(event)->propertyName());
		emit propertyChange(key, value);
		bool result = false;
		int id = key.toInt(&result);
		if (result)
			emit propertyChange(id, value);
	
		break;
	}
	default:
		break;
	}

	return QObject::event(event);
}