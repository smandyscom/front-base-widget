#ifndef DELEGATEVIEWITEMFEATUREMARKER_H
#define DELEGATEVIEWITEMFEATUREMARKER_H

#include <QObject>
#include <QStyledItemDelegate>

class DelegateViewItemFeatureMarker :
        public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit DelegateViewItemFeatureMarker(QObject *parent = nullptr);

    void paint(QPainter *painter,
                   const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;

signals:

public slots:
};

#endif // DELEGATEVIEWITEMFEATUREMARKER_H
