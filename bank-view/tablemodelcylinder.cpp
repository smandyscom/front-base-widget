#include "tablemodelcylinder.h"

TableModelCylinder::TableModelCylinder(QObject *parent) :
    AbstractQVariantSqlTable(parent)
{
    __addressList = {
        ACT_A_1,
        ACT_A_2,
        ACT_B_1,
        ACT_B_2,
        SEN_A_1,
        SEN_A_2,
        SEN_A_3,
        SEN_A_4,
        SEN_B_1,
        SEN_B_2,
        SEN_B_3,
        SEN_B_4,
    };
}

QVariant TableModelCylinder::RowRecord(int rowIndex) const
{
    QSqlRecord __record = record(rowIndex);
    AbstractDataBlock __adb;
    //! fetch by HEADER NAME
    QVariant v = __record.value(QVariant::fromValue(TIMER_SET).value<QString>());

    v = __record.value(QVariant::fromValue(ACT_B_1).value<QString>());
    __adb.Value(TIMER_SET,__record.value(QVariant::fromValue(TIMER_SET).value<QString>()).toReal() * 1000);
    //! Set address
    foreach (Offset var, __addressList) {
        bool isConvertOK;
        QVariant w = __record.value(QVariant::fromValue(var).value<QString>());
        MODBUS_WORD address = __record.value(QVariant::fromValue(var).value<QString>()).value<QString>().toUShort(&isConvertOK,16);
        if(isConvertOK)
            __adb.Value(var,address);
    }
    //! Calculating sensor usage

    return QVariant::fromValue(__adb);
}

void TableModelCylinder::RowRecord(int rowIndex, QVariant value)
{
    QSqlRecord __record = record(rowIndex);
    AbstractDataBlock __adb = value.value<AbstractDataBlock>();
    //!
    __record.setValue(QVariant::fromValue(TIMER_SET).value<QString>(),__adb.Value(TIMER_SET) / 1000);
    //!
    //! Set address
    foreach (Offset var, __addressList) {
        QString address = QString::number(static_cast<MODBUS_ADDRESS>(__adb.Value(var)),16).toUpper();
        __record.setValue(QVariant::fromValue(var).value<QString>(),address);
    }
    setRecord(rowIndex,__record);

    //submit();
}
