#ifndef ABSTRACTQVARIANTSQLTABLE_H
#define ABSTRACTQVARIANTSQLTABLE_H

#include <QObject>
#include <QSqlRelationalTableModel>

#include <QSqlRecord>
#include <QSqlField>

#include <definitionbasicblocks.h>

//!
//! \brief The AbstractSqlTableAdpater class
//! Used to adapting SqlRecord into/from AbstractDataBlock
//! According to inherited type
class AbstractSqlTableAdpater : public QObject
{
    Q_OBJECT
public:
    //!
    //! \brief AbstractSqlTableAdpater
    //! \param parent
    //! Parent should be QSqlTableModel
    AbstractSqlTableAdpater(QObject* parent=nullptr) :
        QObject(parent)
    {
        __model = qobject_cast<QSqlTableModel*>(this->parent());
    }

    void Record(int row, const AbstractDataBlock& data)
    {
        __model->setRecord(row,data2Record(data));
    }
    AbstractDataBlock Record(int row)
    {
        return record2Data(__model->record(row));
    }
protected:
    QSqlTableModel* __model;
    //!
    //! \brief __headerList
    //! Assume that QVariant carried Enum's Name-Value
    QList<QVariant> __headerList;
    AbstractDataBlock* __concreteTypeBlock;

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
