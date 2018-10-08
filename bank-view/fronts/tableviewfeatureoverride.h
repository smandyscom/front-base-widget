#pragma once

#include <definitionauxiliarykeys.h>
#include <tableviewfeature.h>

class TableViewFeatureOverride : public TableViewFeature
{
	Q_OBJECT

public:
	TableViewFeatureOverride(QWidget *parent= nullptr);
	~TableViewFeatureOverride();

protected slots:
void onOverrideClicked(QModelIndex index);
};
