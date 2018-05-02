#ifndef TABLEMODELCYLINDERVISUAL_H
#define TABLEMODELCYLINDERVISUAL_H

#include <QObject>
#include <QMap>
#include <QSqlRecord>
#include <QSqlField>

#include <definitioncylinderblock.h>
#include <utilities.h>

#include <modbuschannel.h>

#include <QSqlRelationalTableModel>

class TableModelCylinderVisual : public QSqlRelationalTableModel
{
    Q_OBJECT
public:
    enum RelatedHeaders
    {
        //! NAME
        ACT_A_1_N,
        ACT_A_2_N,
        ACT_B_1_N,
        ACT_B_2_N,
        SEN_A_1_N,
        SEN_A_2_N,
        SEN_A_3_N,
        SEN_A_4_N,
        SEN_B_1_N,
        SEN_B_2_N,
        SEN_B_3_N,
        SEN_B_4_N,
        //! HAL ADDRESS
        ACT_A_1_H,
        ACT_A_2_H,
        ACT_B_1_H,
        ACT_B_2_H,
        SEN_A_1_H,
        SEN_A_2_H,
        SEN_A_3_H,
        SEN_A_4_H,
        SEN_B_1_H,
        SEN_B_2_H,
        SEN_B_3_H,
        SEN_B_4_H,
    };
    Q_ENUM(RelatedHeaders)


    explicit TableModelCylinderVisual(QSqlRelationalTableModel* source);

    QVariant data(const QModelIndex &idx, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
protected:
    ModbusChannel* __channel;
    QTimer* __timer;

    //!
    //! \brief __map
    //! //! key , IO-Name
    //! //! value ,IO-HalAddress
    QMap<int,int> __map;
    QVector<int> __backGroundColorRole;
};

#endif // TABLEMODELCYLINDERVISUAL_H
