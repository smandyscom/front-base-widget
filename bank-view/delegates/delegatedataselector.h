#ifndef DELEGATEDATASELECTOR_H
#define DELEGATEDATASELECTOR_H

#include <QObject>
#include <QStyledItemDelegate>
#include <QDateEdit>

class DelegateDateSelector : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit DelegateDateSelector(int interestedColumn=0,QObject *parent = nullptr);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const Q_DECL_OVERRIDE;

    void setEditorData(QWidget *editor, const QModelIndex &index) const Q_DECL_OVERRIDE;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const Q_DECL_OVERRIDE;

    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;

signals:

public slots:
protected:
    int __interestedColumn;
};

#endif // DELEGATEDATASELECTOR_H
