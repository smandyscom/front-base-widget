#include "tableviewfeatureoverride.h"

TableViewFeatureOverride::TableViewFeatureOverride(QWidget *parent)
	: TableViewFeature(parent)
{
	connect(this, &TableViewFeatureOverride::clicked,
		this, &TableViewFeatureOverride::onOverrideClicked);
}

TableViewFeatureOverride::~TableViewFeatureOverride()
{
}

void TableViewFeatureOverride::onOverrideClicked(QModelIndex index)
{
	//! Flip user-role
	QAbstractItemModel* model = const_cast<QAbstractItemModel*>(index.model());
	model->setData(index,
		!index.data(HEADER_STRUCTURE::UserRole_MonitorOnOff).toBool(),
		HEADER_STRUCTURE::UserRole_OverrideOnOff);
}
