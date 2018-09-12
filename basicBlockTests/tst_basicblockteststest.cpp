#include <QString>
#include <QtTest>

#include <definitionbasicblocks.h>

class BasicBlockTestsTest : public QObject
{
    Q_OBJECT

public:
    BasicBlockTestsTest();

private Q_SLOTS:
    void testSize();
    void testOperator();
};

BasicBlockTestsTest::BasicBlockTestsTest()
{
}

void BasicBlockTestsTest::testSize()
{
    size_t sz = sizeof(CellDataBlock);
    sz = sizeof(AbstractDataBlock);

    QVERIFY2(true, "Failure");
}

void BasicBlockTestsTest::testOperator()
{
    CellDataBlock data;
    AbstractDataBlock adb1(reinterpret_cast<MODBUS_U_WORD*>(&data));  // as interface of data
    AbstractDataBlock adb2;       // blank
    AbstractDataBlock adb3;

    adb1.Value(2,qrand());

    adb2 = adb1;    //shared the same link
    adb2.Value(3,qrand());

    adb3 = data;    //copy from data

    auto var = adb3.Value(3).toInt();
    adb3.Value(4,qrand());  //wont affect data

    QVERIFY2(true, "Failure");
}


QTEST_APPLESS_MAIN(BasicBlockTestsTest)

#include "tst_basicblockteststest.moc"
