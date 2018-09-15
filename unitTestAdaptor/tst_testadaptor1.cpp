#include <QtTest>

#include <abstractsqltableadpater.h>
#include <commonhelper.h>

#include <definitioncommandblock.h>
// add necessary includes here

class testAdaptor1 : public QObject
{
    Q_OBJECT

public:
    testAdaptor1();
    ~testAdaptor1();

private slots:
    void testWriteRecord();
    void testReadRecord();
protected:
    AbstractSqlTableAdpater* m_adaptor;
};

testAdaptor1::testAdaptor1()
{
    CommonHelper::openSqlite3Database("base.db");
    QSqlTableModel* m_model = new QSqlTableModel(this);
    m_model->setTable("WHOLE_COMMAND_BLOCKS");
    if(!m_model->select())
        qDebug() << "not exited";

    m_adaptor = new GenericSqlTableAdapter<ExtendedCommandBlock,CommandBlock::DataBaseHeaders>(m_model);

    //!
    AbstractDataBlock::Dimension = new QMap<DEF_BASIC_DIMENSION::Keys,qreal>();
    (*AbstractDataBlock::Dimension)[DEF_BASIC_DIMENSION::TIME] = 1;
    (*AbstractDataBlock::Dimension)[DEF_BASIC_DIMENSION::TORQUE_RATIO] = 1;
    (*AbstractDataBlock::Dimension)[DEF_BASIC_DIMENSION::LENGTH] = 1;
}

testAdaptor1::~testAdaptor1()
{
}

void testAdaptor1::testWriteRecord()
{
    CellDataBlock m_cell;
    ExtendedCommandBlock m_extend(reinterpret_cast<MODBUS_U_WORD*>(&m_cell));

    m_extend.Value(ExtendedCommandBlock::OFFSET_ECB_COORD1,1234567);

    //! Write record
    m_adaptor->Record(1,m_extend);
}
void testAdaptor1::testReadRecord()
{
    CellDataBlock m_check;
    AbstractDataBlock* m_extend = new ExtendedCommandBlock(reinterpret_cast<MODBUS_U_WORD*>(&m_check));
    //! Read record
    //! Value assign
    *m_extend = *reinterpret_cast<CellDataBlock*>(m_adaptor->Record(2).Anchor());
}

QTEST_APPLESS_MAIN(testAdaptor1)

#include "tst_testadaptor1.moc"
