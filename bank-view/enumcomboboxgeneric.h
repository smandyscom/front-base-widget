#ifndef ENUMCOMBOBOXGENERIC_H
#define ENUMCOMBOBOXGENERIC_H

#include <QComboBox>
#include <QMetaEnum>
class EnumComboBoxGeneric : public QComboBox
{
    Q_OBJECT
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

    explicit EnumComboBoxGeneric(QWidget *parent = nullptr) : QComboBox(parent){}

signals:

public slots:
};

template<typename T>
class EnumComboBoxGenericTemplate : public EnumComboBoxGeneric
{
public:
    EnumComboBoxGenericTemplate<T>(QWidget *parent = nullptr) : EnumComboBoxGeneric(parent)
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
    //virtual ~name() {}
};

#endif // ENUMCOMBOBOXGENERIC_H
