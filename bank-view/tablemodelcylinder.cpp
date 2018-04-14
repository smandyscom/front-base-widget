#include "tablemodelcylinder.h"

QVariant TableModelCylinder::RowRecord(int rowIndex) const
{
    QSqlRecord __record = record(rowIndex);
    AbstractDataBlock __adb;
    //! fetch by HEADER NAME
    __adb.Value(TIMER_SET,__record.value(QVariant::fromValue(TIMER_SET).value<QString>()).toReal() * 1000);
    //! Set address
    foreach (Offset var, __addressList) {
        bool isConvertOK;
        MODBUS_WORD address = __record.value(QVariant::fromValue(var).value<QString>()).value<QString>().toUShort(&isConvertOK,16);
        if(isConvertOK)
            __adb.Value(var,address);
        else
            __adb.Value(var,0); //set as invalid address
    }
    //! Calculating sensor usage
    MODBUS_WORD counter=0;
    for(int i=SEN_A_1;i<=SEN_A_4;i++)
        if(__adb.Value(i)!=0)
            counter++;
    __adb.Value(A_SENSOR_USED_COUNT,counter);
    //!
    counter=0;
    for(int i=SEN_B_1;i<=SEN_B_4;i++)
        if(__adb.Value(i)!=0)
            counter++;
    __adb.Value(B_SENSOR_USED_COUNT,counter);


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
}
