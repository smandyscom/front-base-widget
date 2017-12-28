#ifndef UTILITIES_H
#define UTILITIES_H

#include <QObject>
#include <QPair>
#include <QItemEditorCreatorBase>
#include <enumcomboboxgeneric.h>
#include <QMetaEnum>
#include <QAbstractItemModel>

typedef QPair<QVariant,QVariant> KeyValuePair;
typedef QList<KeyValuePair> KeyValueCollection;

class utilities
{
public:
    utilities();

    //used to feed into ItemEditorFactorY
    template<typename T> static QPair<int,QItemEditorCreatorBase*> generateEnumComboBoxEditor()
    {
        //warning , allocating new item editor every time , would cause leakage
        //better to use internal hash table instead
        return QPair<int,QItemEditorCreatorBase*>(qRegisterMetaType<T>(),new QStandardItemEditorCreator<EnumComboBoxGenericTemplate<T>>());
    }

    //![1]
    //!
    template<typename T> static QList<QVariant> listupEnumVariant()
    {
        QList<QVariant> output;

        QMetaEnum qme = QMetaEnum::fromType<T>();

        for(int i=0;i<qme.keyCount();i++)
            output.append(QVariant::fromValue(static_cast<T>(qme.value(i))));

        return output;
    }

    static KeyValueCollection model2KeyValuePairs(const QAbstractItemModel &model);

    static int sizeOf(QVariant value);
};

#endif // UTILITIES_H
