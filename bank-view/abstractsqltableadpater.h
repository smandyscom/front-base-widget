#ifndef ABSTRACTQVARIANTSQLTABLE_H
#define ABSTRACTQVARIANTSQLTABLE_H

#include <QObject>
#include <QSqlRelationalTableModel>

#include <QSqlRecord>
#include <QSqlField>
#include <QSqlIndex>
#include <definitionbasicblocks.h>

#include <utilities.h>

#define FIRST_COLUMN 0

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
        //!duplication
        __model = new QSqlTableModel(this,qobject_cast<QSqlTableModel*>(this->parent())->database());
        __model->setTable(qobject_cast<QSqlTableModel*>(this->parent())->tableName());
        __model->select();
    }

    void Record(int key,
                const AbstractDataBlock& data,
                KeyType keyType=KEY_ROW_ID,
                const QVariant keyName=QVariant::fromValue(0))
    {
        __model->setFilter(nullptr);
        int rowIndex = select(key,keyType,keyName);
        __model->setRecord(rowIndex,data2Record(data,rowIndex));
        //!
        //__model->database().transaction();
        //__model->database().commit();
    }
    AbstractDataBlock Record(int key,
                             KeyType keyType=KEY_ROW_ID,
                             const QVariant keyName=QVariant::fromValue(0))
    {
        __model->setFilter(nullptr);
        AbstractDataBlock result = record2Data(__model->record(select(key,keyType,keyName)));
        return result;
    }

    //!
    //! \brief record2Data
    //! \param record
    //! \return
    //! Assume that header list could be on-and-onto data block
    virtual AbstractDataBlock record2Data(const QSqlRecord& record)
    {
        //! clear all data
        *__concreteTypeBlock = AbstractDataBlock();

        foreach (QVariant var, __headerList) {
            if(var.toInt() > INVALID_INDEX)
                __concreteTypeBlock->Value(var.toUInt(),
                                           record.value(utilities::trimNamespace(var)));
        }

        return *__concreteTypeBlock;
    }
    virtual QSqlRecord data2Record(const AbstractDataBlock& data,int referenceRowIndex)
    {
        QSqlRecord __record = __model->record(referenceRowIndex); //get default records as reference
        *__concreteTypeBlock = data; //value assign only , keep vPtr as same

        foreach (QVariant var, __headerList) {
            if(var.toInt() > INVALID_INDEX)
                __record.setValue(utilities::trimNamespace(var),
                                  __concreteTypeBlock->Value(var.toInt()));
        }

        return __record;
    }

    QSqlTableModel* Model() const {return __model;}

protected:
    QSqlTableModel* __model;
    //!
    //! \brief __headerList
    //! Assume that QVariant carried Enum's Name-Value
    QList<QVariant> __headerList;
    AbstractDataBlock* __concreteTypeBlock;


    int select(int key,KeyType keyType,const QVariant keyName){
        switch (keyType) {
        case KEY_ROW_ID:
            return key;
            break;
        case KEY_NAMED_KEY:
            __model->setFilter(utilities::generateFilterString(keyName,QVariant::fromValue(key)));
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
        __concreteTypeBlock = &__dataBlock;
        __headerList = utilities::listupEnumVariant<TypeOfEnumHeader>();
    }
protected:
    TypeOfDataBlock __dataBlock;
};

#endif // ABSTRACTQVARIANTSQLTABLE_H
