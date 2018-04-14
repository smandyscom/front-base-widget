#ifndef ABSTRACTQVARIANTSQLTABLE_H
#define ABSTRACTQVARIANTSQLTABLE_H

#include <QObject>
#include <QSqlRelationalTableModel>

#include <QSqlRecord>
#include <QSqlField>

#include <definitionbasicblocks.h>

//!
//! \brief The AbstractQVariantSqlTable class
//! Turns Row record into/from AbstractDataBlock
class AbstractQVariantSqlTable : public QSqlRelationalTableModel
{
    Q_OBJECT
public:
    explicit AbstractQVariantSqlTable(AbstractQVariantSqlTable* decorator) :
        QSqlRelationalTableModel(decorator->Instance()->parent()),
        __instance(decorator->Instance())
    {

    }

    virtual QVariant RowRecord(int rowIndex) const
    {
        QSqlRecord __record = __instance->record(rowIndex);

        foreach (QVariant var, __wordHeaders) {
            //! Offset , Name
           __block->Value(var.toInt(), __record.value(var.toString()).value<MODBUS_WORD>());
        }
        foreach (QVariant var, __longHeaders) {
            __block->Value(var.toInt(),__record.value(var.toString()).value<MODBUS_LONG>());
        }
        foreach (QVariant var, __quadHeaders) {
            //! Offset , Name
           __block->Value(var.toInt(), __record.value(var.toString()).value<MODBUS_QUAD>());
        }
        foreach (QVariant var, __qrealHeaders) {
            __block->Value(var.toInt(),__record.value(var.toString()).toReal());
        }

        return QVariant::fromValue(*__block);
    }
    virtual void RowRecord(int rowIndex,QVariant value)
    {
        QSqlRecord __record = __instance->record(rowIndex);
        *__block = value.value<AbstractDataBlock>();

        foreach (QVariant var, __wordHeaders) {
            //! Offset , Name
            __record.setValue(var.toString(),__block->Value(var.toInt()).value<MODBUS_WORD>());
        }
        foreach (QVariant var, __longHeaders) {
            __record.setValue(var.toString(),__block->Value(var.toInt()).value<MODBUS_LONG>());
        }
        foreach (QVariant var, __quadHeaders) {
            //! Offset , Name
            __record.setValue(var.toString(),__block->Value(var.toInt()).value<MODBUS_QUAD>());
        }
        foreach (QVariant var, __qrealHeaders) {
            //! Offset , Name
            __record.setValue(var.toString(),__block->Value(var.toInt()).toReal());
        }

        __instance->setRecord(rowIndex,__record);
    }

    //!
    //! \brief Instance
    //! \return 
    //! Offered all posible public interface
    //! Used to dock with native Qt components
    QSqlRelationalTableModel* Instance() const
    {
        return __instance;
    }

protected:
    QSqlRelationalTableModel* __instance;

    AbstractDataBlock* __block; //concretype not decided yet

    QList<QVariant> __wordHeaders;
    QList<QVariant> __longHeaders;
    QList<QVariant> __quadHeaders;
    QList<QVariant> __qrealHeaders;
};
#endif // ABSTRACTQVARIANTSQLTABLE_H
