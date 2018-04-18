#ifndef ABSTRACTQVARIANTSQLTABLE_H
#define ABSTRACTQVARIANTSQLTABLE_H

#include <QObject>
#include <QSqlRelationalTableModel>

#include <QSqlRecord>
#include <QSqlField>
#include <QSqlIndex>
#include <definitionbasicblocks.h>

//!
//! \brief The AbstractSqlTableAdpater class
//! Used to adapting SqlRecord into/from AbstractDataBlock
//! According to inherited type
class AbstractSqlTableAdpater : public QObject
{
    Q_OBJECT
public:
    enum KeyType
    {
        KEY_ROW_ID,
        KEY_NAMED_KEY
    };

    //!
    //! \brief AbstractSqlTableAdpater
    //! \param parent
    //! Parent should be QSqlTableModel
    AbstractSqlTableAdpater(QObject* parent=nullptr) :
        QObject(parent)
    {
        __model = qobject_cast<QSqlTableModel*>(this->parent());
    }

    void Record(int key,
                const AbstractDataBlock& data,
                KeyType keyType=KEY_ROW_ID,
                const QString keyName="")
    {
        __model->setRecord(select(key,keyType,keyName),data2Record(data));
        __model->setFilter(nullptr); //resume all selection
    }
    AbstractDataBlock Record(int key,
                             KeyType keyType=KEY_ROW_ID,
                             const QString keyName="")
    {
        AbstractDataBlock result = record2Data(__model->record(select(key,keyType,keyName)));
        __model->setFilter(nullptr); //resume all selection
        return result;
    }

    //!
    //! \brief record2Data
    //! \param record
    //! \return
    //! Assume that header list could be on-and-onto data block
    virtual AbstractDataBlock record2Data(const QSqlRecord& record)
    {
        foreach (QVariant var, __headerList) {
            __concreteTypeBlock->Value(var.toInt(),record.value(var.toString()));
        }

        return *__concreteTypeBlock;
    }
    virtual QSqlRecord data2Record(const AbstractDataBlock& data)
    {
        QSqlRecord __record = __model->record(); //get empty records as reference
        *__concreteTypeBlock = data;

        foreach (QVariant var, __headerList) {
            __record.setValue(var.toString(),__concreteTypeBlock->Value(var.toInt()));
        }

        return __record;
    }


protected:
    QSqlTableModel* __model;
    //!
    //! \brief __headerList
    //! Assume that QVariant carried Enum's Name-Value
    QList<QVariant> __headerList;
    AbstractDataBlock* __concreteTypeBlock;


    int select(int key,KeyType keyType=KEY_ROW_ID,const QString keyName=""){
        switch (keyType) {
        case KEY_ROW_ID:
            return key;
            break;
        case KEY_NAMED_KEY:
            __model->setFilter(QString("%1=%2").arg(keyName).arg(key));
            return 0; //select first row
            break;
        default:
            return 0;
            break;
        }
    }
};

template<typename TypeOfDataBlock,typename TypeOfEnumHeader>
class GenericSqlTableAdapter : public AbstractSqlTableAdpater
{
public:
    GenericSqlTableAdapter<TypeOfDataBlock,TypeOfEnumHeader>(QObject* parent=nullptr) :
    AbstractSqlTableAdpater(parent)
    {
        __concreteTypeBlock = new TypeOfDataBlock();
        __headerList = utilities::listupEnumVariant<TypeOfEnumHeader>();
    }
};

#endif // ABSTRACTQVARIANTSQLTABLE_H
