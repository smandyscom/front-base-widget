#include <QString>
#include <QtTest>

#include <interfacechannel.h>

class UnitTesterTest2 : public InterfaceChannel
{
    Q_OBJECT

public:
    UnitTesterTest2();

private Q_SLOTS:
    void adsClientTest1();
    void adsClientTest2();
    void adsClientTest3();
};

UnitTesterTest2::UnitTesterTest2() :
    InterfaceChannel(QList<InterfaceClient*>())
{
    this->__clients.append(new InterfaceClient());

    qsrand(QTime::currentTime().msec());
}

void UnitTesterTest2::adsClientTest1()
{
    QVariant __fetch = QVariant::fromValue(true);
    __clients[0]->Cache()[0] = false;
    __update(0x00000000,__fetch);
    QVERIFY2(__fetch.value<bool>() == false,"Failure");

    __fetch = QVariant::fromValue(static_cast<quint16>(0));
    __clients[0]->Cache()[0] = static_cast<quint16>(qrand());
    __update(0x00000000,__fetch);
    QVERIFY2(__fetch.value<quint16>() == __clients[0]->Cache()[0], "Failure");
}

void UnitTesterTest2::adsClientTest2()
{
    __commit(0x00000000,QVariant::fromValue(true));
    __commit(0x00000000,QVariant::fromValue(false));

    __commit(0x00090000,QVariant::fromValue(true));
    __commit(0x00090000,QVariant::fromValue(false));

    __commit(0x000F0000,QVariant::fromValue(true));
    __commit(0x000F0000,QVariant::fromValue(false));

    QVERIFY2(true, "Failure");
}

void UnitTesterTest2::adsClientTest3()
{
    //!Routine
    RegisterRoutines(0x000F0000,QVariant::fromValue(true));
    onAcknowledged(InterfaceRequest(InterfaceRequest::READ,0x000F0000,QVariant::fromValue(true)));

    QVERIFY2(true, "Failure");
}

QTEST_APPLESS_MAIN(UnitTesterTest2)

#include "tst_unittestinterfacechannel.moc"
