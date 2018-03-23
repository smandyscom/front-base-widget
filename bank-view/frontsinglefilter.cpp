#include "frontsinglefilter.h"
#include "ui_frontsinglefilter.h"

FrontSingleFilter::FrontSingleFilter(QSqlTableModel *dataTable,
                                     QSqlTableModel *primaryTable,
                                     QString key,
                                     QString showColumn,
                                     QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrontSingleFilter),
    __dataTable(dataTable),
    __key(key)
{
    ui->setupUi(this);
    //!
    ui->comboBoxPrimary->setModel(primaryTable);
    ui->comboBoxPrimary->setModelColumn(primaryTable->fieldIndex(showColumn));
    ui->comboBoxPrimary->setView(new QTableView(ui->comboBoxPrimary));
    connect(ui->comboBoxPrimary,SIGNAL(currentIndexChanged(int)),this,SLOT(onSelectedIndexChanged(int)));
    //!
    connect(ui->pushButtonSelectAll,SIGNAL(clicked(bool)),this,SLOT(onSelectAll()));
    //!
    ui->labelPrimary->setText(__key);
}

void FrontSingleFilter::onSelectedIndexChanged(int i)
{
    __dataTable->setFilter(tr("%1=%2").arg(__key).arg(i));
}

void FrontSingleFilter::onSelectAll()
{
    __dataTable->select();
}
