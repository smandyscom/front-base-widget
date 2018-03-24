#ifndef TABLEMODELCYLINDER_H
#define TABLEMODELCYLINDER_H

#include <QObject>
#include <abstractqvariantsqltable.h>
#include <basicblocksdefinition.h>
#include <QList>
class TableModelCylinder : public AbstractQVariantSqlTable
{
    Q_OBJECT
public:
    //!
    //! \brief The Offset enum
    //! The offset in AbstracDataBlock
    enum Offset
    {
        TIMER_SET=0,
        A_SENSOR_USED_COUNT=2,
        B_SENSOR_USED_COUNT=3,
        ACT_A_1=4,
        ACT_A_2=5,
        ACT_B_1=6,
        ACT_B_2=7,
        SEN_A_1=8,
        SEN_A_2=9,
        SEN_A_3=10,
        SEN_A_4=11,
        SEN_B_1=12,
        SEN_B_2=13,
        SEN_B_3=14,
        SEN_B_4=15,
    };
    Q_ENUM(Offset)

    explicit TableModelCylinder(QObject *parent = nullptr);

    QVariant RowRecord(int rowIndex) const;
    void RowRecord(int rowIndex,QVariant value);
signals:

public slots:

protected:
    QList<Offset> __addressList;
};

#endif // TABLEMODELCYLINDER_H
