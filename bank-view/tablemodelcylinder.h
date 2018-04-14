#ifndef TABLEMODELCYLINDER_H
#define TABLEMODELCYLINDER_H

#include <QObject>
#include <abstractqvariantsqltable.h>
#include <definitioncylinderblock.h>
class TableModelCylinder : public AbstractQVariantSqlTable
{
    Q_OBJECT
public:
    //!
    //! \brief The Offset enum
    //! The offset in AbstracDataBlock
    enum Offset
    {
        CYL_ID = 0xFFFF,
        //! Data
        TIMER_SET=CylinderContext::OFFSET_CONTEXT_TMR_SET_VALUE,
        A_SENSOR_USED_COUNT=CylinderContext::OFFSET_CONTEXT_A_SENSOR_USED_COUNT,
        B_SENSOR_USED_COUNT=CylinderContext::OFFSET_CONTEXT_B_SENSOR_USED_COUNT,
        ACT_A_1=CylinderContext::OFFSET_CONTEXT_ACT_A_1,
        ACT_A_2=CylinderContext::OFFSET_CONTEXT_ACT_A_2,
        ACT_B_1=CylinderContext::OFFSET_CONTEXT_ACT_B_1,
        ACT_B_2=CylinderContext::OFFSET_CONTEXT_ACT_B_2,
        SEN_A_1=CylinderContext::OFFSET_CONTEXT_SENSOR_A_1,
        SEN_A_2=CylinderContext::OFFSET_CONTEXT_SENSOR_A_2,
        SEN_A_3=CylinderContext::OFFSET_CONTEXT_SENSOR_A_3,
        SEN_A_4=CylinderContext::OFFSET_CONTEXT_SENSOR_A_4,
        SEN_B_1=CylinderContext::OFFSET_CONTEXT_SENSOR_B_1,
        SEN_B_2=CylinderContext::OFFSET_CONTEXT_SENSOR_B_2,
        SEN_B_3=CylinderContext::OFFSET_CONTEXT_SENSOR_B_3,
        SEN_B_4=CylinderContext::OFFSET_CONTEXT_SENSOR_B_4,
    };
    Q_ENUM(Offset)

    explicit TableModelCylinder(AbstractQVariantSqlTable* decorator) :
        AbstractQVariantSqlTable(decorator)
    {
        __block = CylinderContext();
        __qrealHeaders = {
            QVariant::fromValue(TIMER_SET)
        };
        __wordHeaders = {
            QVariant::fromValue(A_SENSOR_USED_COUNT),
            QVariant::fromValue(B_SENSOR_USED_COUNT),
            QVariant::fromValue(ACT_A_1),
            QVariant::fromValue(ACT_A_2),
            QVariant::fromValue(ACT_B_1),
            QVariant::fromValue(ACT_B_2),
            QVariant::fromValue(SEN_A_1),
            QVariant::fromValue(SEN_A_2),
            QVariant::fromValue(SEN_A_3),
            QVariant::fromValue(SEN_A_4),
            QVariant::fromValue(SEN_B_1),
            QVariant::fromValue(SEN_B_2),
            QVariant::fromValue(SEN_B_3),
            QVariant::fromValue(SEN_B_4),
        };
    }

    QVariant RowRecord(int rowIndex) const;
    void RowRecord(int rowIndex,QVariant value);

};

#endif // TABLEMODELCYLINDER_H
