#include <QString>
#include <QtTest>

#include <definitionslotblock.h>

class UnitTesterTest3 : public QObject
{
    Q_OBJECT

public:
    UnitTesterTest3();

private Q_SLOTS:
    void test1();
};

UnitTesterTest3::UnitTesterTest3() :
    QObject(qApp)
{

}

void UnitTesterTest3::test1()
{
    MODBUS_U_WORD buffer[128];
    memset(&buffer,0,sizeof(buffer));
    CellDataBlock data;
    AbstractDataBlock adb1(reinterpret_cast<MODBUS_U_WORD*>(&data));
    adb1.Value(32,555);

    SlotDataBlock* sb = new SlotDataBlock(reinterpret_cast<MODBUS_U_WORD*>(&buffer),qApp);

    sb->Value(SlotDataBlock::BLOCK_DATA,QVariant::fromValue(data));

    CellDataBlock dataReturn = sb->Value(SlotDataBlock::BLOCK_DATA).value<CellDataBlock>();
    AbstractDataBlock adb2(reinterpret_cast<MODBUS_U_WORD*>(&dataReturn));

    QVERIFY2(true, "Failure");
}

QTEST_APPLESS_MAIN(UnitTesterTest3)

#include "tst_unittestertest3.moc"
