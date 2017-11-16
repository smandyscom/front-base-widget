#ifndef ENUMITEMDELEGATE_H
#define ENUMITEMDELEGATE_H

#include <QComboBox>
#include <QStyledItemDelegate>
#include <enumcombobox.h>

template<typename T>
class EnumItemDelegate : public QStyledItemDelegate
{
public:
    EnumItemDelegate(QWidget *parent = 0) : QStyledItemDelegate(parent) {}

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override
    {
        return EnumComboBoxTemplate<T>(parent);
    }
    void setEditorData(QWidget *editor, const QModelIndex &index) const override
    {
        static_cast<EnumComboBoxTemplate<T>*>(editor)->setValue(index.model()->data(index).value<T>());
    }
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override
    {
      model->setData(index,QVariant::fromValue(static_cast<EnumComboBoxTemplate<T>*>(editor)->readValue()))
    }
};


#endif // ENUMITEMDELEGATE_H
