#include "enumcombobox.h"

template<class T>
EnumComboBox<T>::EnumComboBox(QWidget *parent) : QComboBox(parent)
{
    //object:
    // 1. load all items from the definition of enum , including its key/value
    // 2. when user selected specific item


    QMetaEnum target = QMetaEnum::fromType<T>();

    for(int i=0;i<target.keyCount();i++)
        insertItem(i,QString(target.key(i)),target.value(i));
}
