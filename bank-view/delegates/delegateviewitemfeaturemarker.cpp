#include "delegateviewitemfeaturemarker.h"

DelegateViewItemFeatureMarker::DelegateViewItemFeatureMarker(QObject *parent) :
    QStyledItemDelegate(parent)
{

}

void DelegateViewItemFeatureMarker::paint(QPainter *painter,
                                          const QStyleOptionViewItem &option,
                                          const QModelIndex &index) const
{
    //!
    //! \brief opt
    //! Set Style option by UserRole
    QStyleOptionViewItem opt = option;
    opt.features.setFlag(QStyleOptionViewItem::Alternate,index.data(Qt::UserRole).toBool());
    QStyledItemDelegate::paint(painter,opt,index);
}
