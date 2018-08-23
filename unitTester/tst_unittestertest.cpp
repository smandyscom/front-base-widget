#include <QString>
#include <QtTest>

#include <adsclient.h>

class UnitTesterTest : public AdsClient
{
    Q_OBJECT

public:
    UnitTesterTest();

private Q_SLOTS:
    void adsClientTest();
};

UnitTesterTest::UnitTesterTest() :
    AdsClient(AmsAddr(),true,nullptr)
{

}

void UnitTesterTest::adsClientTest()
{
    //AmsAddr __addr;
    //AdsClient* __client = new AdsClient(__addr,true,nullptr);
    uint cache=0;
    pushRequest(AdsRequest(AdsRequest::READ,0x4020,127,sizeof(bool),&cache));
    executeRequest();

    QVERIFY2(true, "Failure");
}

QTEST_APPLESS_MAIN(UnitTesterTest)

#include "tst_unittestertest.moc"
