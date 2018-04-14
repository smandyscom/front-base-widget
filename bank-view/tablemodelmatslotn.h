#ifndef TABLEMODELMATSLOTN_H
#define TABLEMODELMATSLOTN_H

#include <QObject>
#include <abstractqvariantsqltable.h>

class TableModelMatSlotN : public AbstractQVariantSqlTable
{
    Q_OBJECT
public:
    explicit TableModelMatSlotN(QObject *parent = nullptr);

signals:

public slots:
};

#endif // TABLEMODELMATSLOTN_H
