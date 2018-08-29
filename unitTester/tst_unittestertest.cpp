#include <QString>
#include <QtTest>

#include <adsclient.h>

class UnitTesterTest : public AdsClient
{
    Q_OBJECT

public:
    UnitTesterTest();

private Q_SLOTS:
    void adsClientTest1();
    void adsClientTest2();
};

UnitTesterTest::UnitTesterTest() :
    AdsClient(AmsAddr(),true,0x4020,nullptr)
{

}

void UnitTesterTest::adsClientTest1()
{
    //AmsAddr __addr;
    //AdsClient* __client = new AdsClient(__addr,true,nullptr);
    uint cache=0;
    pushRequest(InterfaceRequest(InterfaceRequest::READ,0,QVariant::fromValue(static_cast<quint16>(0))));
    executeRequest();

    QVERIFY2(true, "Failure");
}

void UnitTesterTest::adsClientTest2()
{
    //AmsAddr __addr;
    //AdsClient* __client = new AdsClient(__addr,true,nullptr);
    uint cache=0;
    pushRequest(InterfaceRequest(InterfaceRequest::READ,0,QVariant::fromValue(static_cast<quint32>(0))));
    executeRequest();

    QVERIFY2(true, "Failure");
}

QTEST_APPLESS_MAIN(UnitTesterTest)

#include "tst_unittestertest.moc"
