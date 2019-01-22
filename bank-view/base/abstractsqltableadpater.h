#ifndef ABSTRACTQVARIANTSQLTABLE_H
#define ABSTRACTQVARIANTSQLTABLE_H

#include <QObject>
#include <QSqlTableModel>

#include <QSqlRecord>
#include <QSqlField>
#include <QSqlIndex>
#include <definitionbasicblocks.h>

#include <utilities.h>

#include <QDebug>
#include <QSqlError>
#define FIRST_COLUMN 0

//!
//! \brief The AbstractSqlTableAdpater class
//! Used to adapting SqlRecord into/from AbstractDataBlock
//! According to inherited type
class AbstractSqlTableAdpater :
        public QObject
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
    AbstractSqlTableAdpater(QSqlTableModel* table,
                            QList<QVariant> headerList,
                            AbstractDataBlock* concreteBlock) :
        QObject(table),
        m_model(table),
        m_headerList(headerList),
        m_concreteBlock(concreteBlock)
    {
        //! directly use parent rather create new one?
		m_model->select(); //population first time
    }
    //!
    //! \brief Record
    //! \param key
    //! \param data
    //! \param keyType
    //! \param keyName
    //! Write record by datablock
    void Record(int key,
                const AbstractDataBlock& data,
                KeyType keyType=KEY_ROW_ID,
                const QVariant keyName=QVariant::fromValue(0))
    {

        int rowIndex = select(key,keyType,keyName);

        //! multi client write-in should use truncate?
        if(!m_model->setRecord(rowIndex,data2Record(data,rowIndex)))
        {
            qDebug() << QString("%1:setRecord failed").arg(rowIndex);
            qDebug() << QString("%1").arg(m_model->lastError().text());
        }
    }
    //!
    //! \brief Record
    //! \param key
    //! \param keyType
    //! \param keyName
    //! \return
    //! Read record and write into data block
    AbstractDataBlock& Record(int key,
                             KeyType keyType=KEY_ROW_ID,
                             const QVariant keyName=QVariant::fromValue(0))
    {
        return record2Data(m_model->record(select(key,keyType,keyName)));
    }

    //!
    //! \brief record2Data
    //! \param record
    //! \return
    //! Assume that header list could be on-and-onto data block
    virtual AbstractDataBlock& record2Data(const QSqlRecord& record)
    {
        foreach (QVariant var, m_headerList) {
            if(var.toInt() > INVALID_INDEX)
                m_concreteBlock->Value(var.toUInt(),
                                           record.value(utilities::trimNamespace(var)));
        }

        return *m_concreteBlock;
    }
    virtual QSqlRecord data2Record(const AbstractDataBlock& data,int referenceRowIndex)
    {
        QSqlRecord m_record = m_model->record(referenceRowIndex); //get default records as reference
        *m_concreteBlock = *reinterpret_cast<CellDataBlock*>(data.Anchor()); //value copy

        //! Set by column name
        foreach (QVariant var, m_headerList) {
            if(var.toInt() > INVALID_INDEX)
                m_record.setValue(utilities::trimNamespace(var),
                                  m_concreteBlock->Value(var.toInt()));
        }

        return m_record;
    }


    QSqlTableModel* Model() const {return m_model;}
	void Model(QSqlTableModel* model) { 
		m_model = model; 
		m_model->select();//first time population
	}

protected:
    QSqlTableModel* m_model;
    //!
    //! \brief __headerList
    //! Assume that QVariant carried Enum's Name-Value
    QList<QVariant> m_headerList;
    AbstractDataBlock* m_concreteBlock;


    int select(int key,KeyType keyType,const QVariant keyName){

                int m_index = 0;

        switch (keyType) {
        case KEY_ROW_ID:
            m_model->setFilter(nullptr);
            m_index = key;
            break;
        case KEY_NAMED_KEY:
            m_model->setFilter(utilities::generateFilterString(keyName,QVariant::fromValue(key)));
            m_index = 0;
            break;
        default:
            break;
        }

        //m_model->select(); //update before operating
        return m_index;
    }
};

template<typename TypeOfDataBlock,typename TypeOfEnumHeader>
class GenericSqlTableAdapter :
        public AbstractSqlTableAdpater
{
public:
    GenericSqlTableAdapter<TypeOfDataBlock,TypeOfEnumHeader>(QSqlTableModel* table) :
    AbstractSqlTableAdpater(table,
                            utilities::listupEnumVariant<TypeOfEnumHeader>(),
                            new TypeOfDataBlock(table))
    {
        //! Allocating
    }
};

#endif // ABSTRACTQVARIANTSQLTABLE_H
