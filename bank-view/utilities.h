#ifndef UTILITIES_H
#define UTILITIES_H

#include <QObject>
#include <QPair>
#include <QItemEditorCreatorBase>
#include <enumcombobox.h>
#include <enumcomboboxgeneric.h>
class utilities
{
public:
    utilities();

    //used to feed into ItemEditorFactorY
    template<typename T> static QPair<int,QItemEditorCreatorBase*> generateEnumComboBoxEditor()
    {
        //warning , allocating new item editor every time , would cause leakage
        //better to use internal hash table instead
        //return QPair<int,QItemEditorCreatorBase*>(qRegisterMetaType<T>(),new QStandardItemEditorCreator<EnumComboBoxTemplate<T>>());
        return QPair<int,QItemEditorCreatorBase*>(qRegisterMetaType<T>(),new QStandardItemEditorCreator<EnumComboBoxGenericTemplate<T>>());
    }
};

#endif // UTILITIES_H
