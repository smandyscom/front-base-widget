#ifndef ENUMCOMBOBOX_H
#define ENUMCOMBOBOX_H

#include <QObject>
#include <QWidget>
#include <QComboBox>
#include <QMetaEnum>
#include <bank-view_global.h>


template<class T>
class BANKVIEWSHARED_EXPORT EnumComboBox : public QComboBox
{
public:
    EnumComboBox<T>(QWidget *parent = nullptr):QComboBox(parent){
        //object:
        // 1. load all items from the definition of enum , including its key/value
        // 2. when user selected specific item


        QMetaEnum target = QMetaEnum::fromType<T>();

        for(int i=0;i<target.keyCount();i++)
            insertItem(i,QString(target.key(i)),target.value(i));
    }
};

#endif // ENUMCOMBOBOX_H
