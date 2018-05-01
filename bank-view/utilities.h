#ifndef UTILITIES_H
#define UTILITIES_H

#include <QObject>
#include <QPair>
#include <QItemEditorCreatorBase>
#include <enumcomboboxgeneric.h>
#include <QMetaEnum>
#include <QAbstractItemModel>
#include <QColor>
#include <QPalette>
#include <QTableView>
#include <QComboBox>
#include <QListView>

#include <QSqlTableModel>
#include <QSqlRecord>

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
    static void colorChangeOver(QWidget* target, bool value, Qt::GlobalColor trueColor=Qt::green, Qt::GlobalColor falseColor=Qt::gray);

    static int getSelectedValue(QTableView* target,const QString& keyName);
    static QSqlRecord getSqlTableSelectedRecord(QSqlTableModel* target,QVariant keyName,QVariant keyValue);
    static QString generateFilterString(QVariant keyName,QVariant keyValue);

    static QString trimNamespace(QVariant key);

    static void linkQComboBoxAndModel(QComboBox* comboBox,
                                      QSqlTableModel *model,
                                      QVariant showKey = QString("NAME"));
};

#endif // UTILITIES_H
