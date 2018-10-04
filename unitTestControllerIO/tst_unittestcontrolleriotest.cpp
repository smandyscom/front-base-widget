#include <QString>
#include <QtTest>
#include <QDir>
#include <QDebug>
#include <controlleriomonitor.h>

class UnitTestControllerIOTest : public ControllerIOMonitor
{
    Q_OBJECT

public:
    UnitTestControllerIOTest();

private Q_SLOTS:
    void testCase1();
};

UnitTestControllerIOTest::UnitTestControllerIOTest() :
    ControllerIOMonitor(0)
{
}

void UnitTestControllerIOTest::testCase1()
{
    qDebug() << QDir::currentPath();

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("base.db");
    if(!db.open())
        qDebug() << "fail";
    //!
    QSqlTableModel* model = new QSqlTableModel();
    model->setTable("WHOLE_INPUTS");
    if(!model->select())
        qDebug() << "fail";
    QMap<QVariant,QVariant> map;
    map["HAL_ADDRESS"] = "NAME";

    setModel(model,map);

    QVERIFY2(true, "Failure");
}

QTEST_APPLESS_MAIN(UnitTestControllerIOTest)

#include "tst_unittestcontrolleriotest.moc"
