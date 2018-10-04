#include "delegateviewitemfeaturemarker.h"
#include <QDebug>
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

    if(index.data(Qt::UserRole).toBool())
        qDebug() << "true;";

    QStyledItemDelegate::paint(painter,opt,index);
}
