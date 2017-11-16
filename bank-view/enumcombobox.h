#ifndef ENUMCOMBOBOX_H
#define ENUMCOMBOBOX_H

#include <QObject>
#include <QWidget>
#include <QComboBox>
#include <QMetaEnum>
#include <bank-view_global.h>


template<typename T>
class EnumComboBoxTemplate : public QComboBox
{
public:
    T readValue() const
    {
        return currentData().value();
    }
    void setValue(T value)
    {
        setCurrentIndex(findData(QVariant::fromValue(value)));
    }

    EnumComboBoxTemplate<T>(QWidget *parent = nullptr) : QComboBox(parent)
    {
        //object:
        // 1. load all items from the definition of enum , including its key/value
        // 2. when user selected specific item
        QMetaEnum target = QMetaEnum::fromType<T>();

        for(int i=0;i<target.keyCount();i++){
            insertItem(i
                       ,QString(target.key(i))
                       ,QVariant::fromValue(static_cast<T>(target.value(i))));
        }
    }

};

//class EnumComboBoxDynamic : public QComboBox
//{
//    Q_PROPERTY(QVariant value READ readValue WRITE setValue USER true)
//    Q_OBJECT
//public:
//    QVariant readValue() const;
//    void setValue(QVariant value);
//    EnumComboBoxDynamic(QMetaEnum target,QWidget *parent);
//    //virtual ~name() {}
//protected:
//    QVariant __seed; // stored the enum object
//};


#endif // ENUMCOMBOBOX_H
