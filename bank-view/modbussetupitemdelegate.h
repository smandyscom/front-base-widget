#ifndef MODBUSSETUPITEMDELEGATE_H
#define MODBUSSETUPITEMDELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>

class modbusSetupItemDelegate : public QStyledItemDelegate
{
public:
    modbusSetupItemDelegate(QWidget *parent);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override
    {
        //return EnumComboBoxTemplate<T>(parent);
    }
    void setEditorData(QWidget *editor, const QModelIndex &index) const override
    {
        //static_cast<EnumComboBoxTemplate<T>*>(editor)->setValue(index.model()->data(index).value<T>());
    }
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override
    {
      //model->setData(index,QVariant::fromValue(static_cast<EnumComboBoxTemplate<T>*>(editor)->readValue()))
    }
protected:
    //cache
};

#endif // MODBUSSETUPITEMDELEGATE_H
