#pragma once

#include <QObject>
#include <qevent.h>
#include <qvariant.h>

//! Turns property into/from signal/slot (Cross-thread usage
class PropertyPortCommon : public QObject
{
	Q_OBJECT

public:
	PropertyPortCommon(QObject *parent=nullptr);
	~PropertyPortCommon();

	//bool event(QEvent* event) Q_DECL_OVERRIDE;

signals:
	void externalPropertyChange(QVariant key,QVariant value);

	void internalPropertyChange(QVariant key, QVariant value);
public slots:
	void onPropertyChanged(QVariant key, QVariant value);
};
