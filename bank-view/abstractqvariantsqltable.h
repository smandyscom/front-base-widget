#ifndef ABSTRACTQVARIANTSQLTABLE_H
#define ABSTRACTQVARIANTSQLTABLE_H

#include <QObject>
#include <QSqlRelationalTableModel>
#include <QSqlRecord>

class AbstractQVariantSqlTable : public QSqlRelationalTableModel
{
    Q_OBJECT
public:
    explicit AbstractQVariantSqlTable(QObject *parent = Q_NULLPTR) :
        QSqlRelationalTableModel(parent)
    {

    }

    virtual QVariant RowRecord(int rowIndex) const
    {
        return record(rowIndex).value(0);
    }
    virtual void RowRecord(int rowIndex,QVariant value)
    {
        QSqlRecord __record;
        __record.setValue(0,value);
        setRecord(rowIndex,__record);
    }

signals:

public slots:
};
#endif // ABSTRACTQVARIANTSQLTABLE_H
