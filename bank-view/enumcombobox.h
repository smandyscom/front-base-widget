#ifndef ENUMCOMBOBOX_H
#define ENUMCOMBOBOX_H

#include <QObject>
#include <QWidget>
#include <QComboBox>
#include <QMetaEnum>
#include <bank-view_global.h>


template<typename T>
class EnumComboBox : public QComboBox
{
    Q_PROPERTY(QVariant value READ readValue WRITE setValue USER true)
public:
    QVariant readValue() const
    {
        return currentData();
    }
    void setValue(QVariant value)
    {
        setCurrentIndex(findData(value));
    }

    EnumComboBox<T>(QWidget *parent = nullptr) : QComboBox(parent)
    {
        //object:
        // 1. load all items from the definition of enum , including its key/value
        // 2. when user selected specific item


        QMetaEnum target = QMetaEnum::fromType<T>();

        for(int i=0;i<target.keyCount();i++){
            //T var = target.value(i);
            //var.setValue(target.value(i));
            insertItem(i
                       ,QString(target.key(i))
                       ,qvariant_cast<T>(target.value(i)));
        }


    }

};

#endif // ENUMCOMBOBOX_H
