#include "fronttwinfilter2.h"
#include "ui_fronttwinfilter2.h"

FrontTwinFilter2::FrontTwinFilter2(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrontTwinFilter2),
    m_value1(0),
    m_value2(0)
{
    ui->setupUi(this);

}

void FrontTwinFilter2::Setup(QSqlTableModel *dataTable, QVariant key1, QSqlTableModel *keyTable1, QVariant key2, QSqlTableModel *keyTable2)
{
    m_dataTable = dataTable;
    m_keyTable1 = keyTable1;
    m_keyTable2 = keyTable2;
    m_key1 = key1;
    m_key2 = key2;
    //!
    ui->comboBoxSecondary->setModel(m_keyTable2);
    ui->comboBoxPrimary->setModel(m_keyTable1);
    m_manipulateMap[ui->pushButtonPrimarySelectAll] = m_dataTable;
    m_manipulateMap[ui->pushButtonSecondary] = m_keyTable1;
    m_manipulateMap[ui->comboBoxPrimary] = m_keyTable1;
    m_manipulateMap[ui->comboBoxSecondary] = m_keyTable2;
    //!
    QList<QComboBox*> m_list = {ui->comboBoxPrimary,
                                ui->comboBoxSecondary};
    foreach (QComboBox* var, m_list) {
        var->clear();
        var->setModel(m_manipulateMap[var]);
        QListView* qtv = new QListView(var);
        var->setView(qtv);
        //! Hide all but NAME
        int m_nameColumnIndex = m_manipulateMap[var]->fieldIndex("zh_TW");

        connect(var,SIGNAL(currentIndexChanged(int)),this,SLOT(onSelectionChanged(int)));
        var->setModelColumn(m_nameColumnIndex);
        var->setCurrentIndex(0);
    }
    //!
    QList<QPushButton*> m_buttonList = {ui->pushButtonPrimarySelectAll,
                                        ui->pushButtonSecondary};
    foreach (QPushButton* var, m_buttonList) {
        connect(var,&QPushButton::clicked,this,&FrontTwinFilter2::onSelectAll);
    }
}

FrontTwinFilter2::~FrontTwinFilter2()
{
    delete ui;
}

void FrontTwinFilter2::onSelectionChanged(int i)
{
    if(sender() == ui->comboBoxPrimary)
    {
        m_value1 = m_keyTable1->record(ui->comboBoxPrimary->currentIndex()).value(FIRST_COLUMN);
        m_dataTable->setFilter(utilities::generateFilterString(m_key1,m_value1));
        m_dataTable->select();
        //! Filter backup
        m_filter1 = m_dataTable->filter();
        emit key1Selected(m_value1);
    }
    else if(sender() == ui->comboBoxSecondary)
    {
        m_value2 = m_keyTable2->record(ui->comboBoxSecondary->currentIndex()).value(FIRST_COLUMN);
        m_keyTable1->setFilter(utilities::generateFilterString(m_key2,m_value2));
        m_keyTable1->select();
        m_filter2 = m_keyTable2->filter();
        emit key2Selected(m_value2);
    }
}
void FrontTwinFilter2::onSelectAll()
{
    m_manipulateMap[qobject_cast<QWidget*>(sender())]->setFilter(nullptr);
}
