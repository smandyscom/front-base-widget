#include "delegatedataselector.h"

DelegateDateSelector::DelegateDateSelector(int interestedColumn, QObject *parent) :
    QStyledItemDelegate(parent),
    __interestedColumn(interestedColumn)
{

}


QWidget* DelegateDateSelector::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.column()==__interestedColumn)
        return new QDateEdit(parent);

   return QStyledItemDelegate::createEditor(parent,option,index);
}

void DelegateDateSelector::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QDateEdit* __dateEdit = qobject_cast<QDateEdit*>(editor);
    __dateEdit->setDate(index.model()->data(index).toDate());
}

void DelegateDateSelector::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QDateEdit* __dateEdit = qobject_cast<QDateEdit*>(editor);
    model->setData(index,QVariant::fromValue(__dateEdit->date()));
}

void DelegateDateSelector::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}
