#include "enumcomboboxdynamic.h"

EnumComboBoxDynamic::EnumComboBoxDynamic(QMetaEnum target,CastingFunction cast,QWidget *parent) : QComboBox(parent)
{
    //object:
    // 1. load all items from the definition of enum , including its key/value
    // 2. when user selected specific item
    for(int i=0;i<target.keyCount();i++){
        insertItem(i
                   ,QString(target.key(i))
                   ,(*cast)(target.value(i)));
    }
}


QVariant EnumComboBoxDynamic::readValue() const
{
    return currentData(Qt::EditRole);
}
void EnumComboBoxDynamic::setValue(QVariant value)
{
    setCurrentIndex(findData(value));
}
