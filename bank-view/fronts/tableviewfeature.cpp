#include "tableviewfeature.h"

TableViewFeature::TableViewFeature(QWidget *parent) :
    QTableView(parent)
{
    setItemDelegate(new DelegateViewItemFeatureMarker(this));
}
