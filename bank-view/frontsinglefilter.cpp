#include "frontsinglefilter.h"
#include "ui_frontsinglefilter.h"

FrontSingleFilter::FrontSingleFilter(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrontSingleFilter)
{
    ui->setupUi(this);
    //!
    __dataKey = QString("REGION"); //default
    ui->labelPrimary->setText(__dataKey.toString());
    //!
    connect(ui->comboBoxPrimary,SIGNAL(activated(int)),this,SLOT(onSelectedIndexChanged(int)));
    connect(ui->pushButtonSelectAll,SIGNAL(clicked(bool)),this,SLOT(onSelectAll()));
    //!
}

void FrontSingleFilter::onSelectedIndexChanged(int i)
{
    QSqlTableModel* __primaryTable = reinterpret_cast<QSqlTableModel*>(ui->comboBoxPrimary->model());
    int id = __primaryTable->record(i).value(QVariant::fromValue(ID).toString()).toInt();

   __dataTable->setFilter( utilities::generateFilterString(__dataKey,QVariant::fromValue(id)));
}

void FrontSingleFilter::onSelectAll()
{
    __dataTable->setFilter(nullptr);
    __dataTable->select();
}

void FrontSingleFilter::PrimaryTable(QSqlTableModel *model, QVariant showKey)
{
    utilities::linkQComboBoxAndModel(ui->comboBoxPrimary,model,showKey);
}

void FrontSingleFilter::DataKey(QVariant key)
{
    __dataKey = key;
    ui->labelPrimary->setText(__dataKey.toString());
}
