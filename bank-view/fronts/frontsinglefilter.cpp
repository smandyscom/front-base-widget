#include "frontsinglefilter.h"
#include "ui_frontsinglefilter.h"

FrontSingleFilter::FrontSingleFilter(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrontSingleFilter)
{
    ui->setupUi(this);
    //!
    m_dataKey = QString("REGION"); //default
    ui->labelPrimary->setText(m_dataKey.toString());
    //!
    connect(ui->comboBoxPrimary,SIGNAL(activated(int)),this,SLOT(onSelectedIndexChanged(int)));
    connect(ui->pushButtonSelectAll,SIGNAL(clicked(bool)),this,SLOT(onSelectAll()));
    //!
}

void FrontSingleFilter::onSelectedIndexChanged(int i)
{
    QSqlTableModel* m_primaryTable = reinterpret_cast<QSqlTableModel*>(ui->comboBoxPrimary->model());
    int id = m_primaryTable->record(i).value(QVariant::fromValue(ID).toString()).toInt();

   m_dataTable->setFilter( utilities::generateFilterString(m_dataKey,QVariant::fromValue(id)));
   m_dataTable->select();
}

void FrontSingleFilter::onSelectAll()
{
    m_dataTable->setFilter(nullptr);
    m_dataTable->select();
}

void FrontSingleFilter::PrimaryTable(QSqlTableModel *model, QVariant showKey)
{
    utilities::linkQComboBoxAndModel(ui->comboBoxPrimary,model,showKey);
}

void FrontSingleFilter::DataKey(QVariant key)
{
    m_dataKey = key;
    ui->labelPrimary->setText(m_dataKey.toString());
}

void FrontSingleFilter::DataTable(QSqlTableModel *model)
{
    m_dataTable = model;
}
