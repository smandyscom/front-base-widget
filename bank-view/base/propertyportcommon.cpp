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
	emit internalPropertyChange(key, value);
}