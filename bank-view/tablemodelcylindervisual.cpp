#include "tablemodelcylindervisual.h"

TableModelCylinderVisual::TableModelCylinderVisual(QSqlRelationalTableModel *source) :
    QSqlRelationalTableModel(source->parent(),source->database())
{
    //!
    __backGroundColorRole.append(Qt::BackgroundColorRole);
    //! Copy its selection
    setTable(source->tableName());
    select();
    source->deleteLater();
    __channel = ModbusChannel::Instance();
    //! Forced to inform View to update periodically
    __timer = new QTimer(this);
    connect(__timer,&QTimer::timeout,[=](){
       emit dataChanged(index(0,0),index(rowCount(),columnCount()),__backGroundColorRole);
    });
    __timer->start();
    //! Build map according to column name
    QMap<QVariant,QVariant> __columnPairQVariant;
    __columnPairQVariant[QVariant::fromValue(ACT_A_1_N)] = QVariant::fromValue(ACT_A_1_H);
    __columnPairQVariant[QVariant::fromValue(ACT_A_2_N)] = QVariant::fromValue(ACT_A_2_H);
    __columnPairQVariant[QVariant::fromValue(ACT_B_1_N)] = QVariant::fromValue(ACT_B_1_H);
    __columnPairQVariant[QVariant::fromValue(ACT_B_2_N)] = QVariant::fromValue(ACT_B_2_H);

    __columnPairQVariant[QVariant::fromValue(SEN_B_1_N)] = QVariant::fromValue(SEN_B_1_H);
    __columnPairQVariant[QVariant::fromValue(SEN_B_2_N)] = QVariant::fromValue(SEN_B_2_H);
    __columnPairQVariant[QVariant::fromValue(SEN_B_3_N)] = QVariant::fromValue(SEN_B_3_H);
    __columnPairQVariant[QVariant::fromValue(SEN_B_4_N)] = QVariant::fromValue(SEN_B_4_H);

    __columnPairQVariant[QVariant::fromValue(SEN_A_1_N)] = QVariant::fromValue(SEN_A_1_H);
    __columnPairQVariant[QVariant::fromValue(SEN_A_2_N)] = QVariant::fromValue(SEN_A_2_H);
    __columnPairQVariant[QVariant::fromValue(SEN_A_3_N)] = QVariant::fromValue(SEN_A_3_H);
    __columnPairQVariant[QVariant::fromValue(SEN_A_4_N)] = QVariant::fromValue(SEN_A_4_H);


    foreach (QVariant nameColumn, __columnPairQVariant.keys()) {
        __map[fieldIndex(nameColumn.toString())] = fieldIndex(__columnPairQVariant[nameColumn].toString());
    }
    //!
}

QVariant TableModelCylinderVisual::data(const QModelIndex &idx, int role) const
{
    //! irrelavent columns
    if(!__map.keys().contains(idx.column()) || role != Qt::BackgroundColorRole)
        return QSqlRelationalTableModel::data(idx,role);

    return __channel->Access<bool>(ModbusDriverAddress(index(idx.row(),__map[idx.column()]).data().toUInt()))?
                QColor(Qt::green) :
                QColor(Qt::gray);
}
