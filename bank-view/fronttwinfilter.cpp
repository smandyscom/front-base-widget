#include "fronttwinfilter.h"
#include "ui_fronttwinfilter.h"

FrontTwinFilter::FrontTwinFilter(QSqlRelationalTableModel *dataTable,
                                 QVariant key1,
                                 QSqlRelationalTableModel *primaryTable,
                                 QVariant key2,
                                 QSqlRelationalTableModel *secondaryTable,
                                 QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrontTwinFilter),
    __dataTable(dataTable),
    __key1(key1.toString()),
    __primaryTable(primaryTable),
    __key2(key2.toString()),
    __secondaryTable(secondaryTable)
{
    ui->setupUi(this);

    //!
    ui->comboBoxSecondary->setModel(__secondaryTable);
    ui->comboBoxPrimary->setModel(__primaryTable);
    __manipulateMap[ui->pushButtonPrimarySelectAll] = __dataTable;
    __manipulateMap[ui->pushButtonSecondary] = __primaryTable;
    //!
    QList<QComboBox*> __list = {ui->comboBoxPrimary,
                                ui->comboBoxSecondary};
    foreach (QComboBox* var, __list) {
        QSqlRelationalTableModel* __model = qobject_cast<QSqlRelationalTableModel*>(var->model());
        var->setModel(__model);
        var->setView(new QTableView(var));
        connect(var,SIGNAL(currentIndexChanged(int)),this,SLOT(onSelectionChanged(int)));
    }
    //!
    QList<QPushButton*> __buttonList = {ui->pushButtonPrimarySelectAll,
                                        ui->pushButtonSecondary};
    foreach (QPushButton* var, __buttonList) {
        connect(var,&QPushButton::clicked,this,&FrontTwinFilter::onSelectAll);
    }
}

FrontTwinFilter::~FrontTwinFilter()
{
    delete ui;
}


void FrontTwinFilter::onSelectionChanged(int i)
{
    QVariant value;

    if(sender() == ui->comboBoxPrimary)
    {
        value = __primaryTable->record(i).value(__primaryTable->primaryKey().name());
        __dataTable->setFilter(QString("%1=\'%2\'").arg(__key1).arg(value.toString()));

        emit primarySelected(value);
    }
    else if(sender() == ui->comboBoxSecondary)
    {
        value = __secondaryTable->record(i).value(__secondaryTable->primaryKey().name());
        __primaryTable->setFilter(QString("%1=\'%2\'").arg(__key2).arg(value.toString()));
    }
}
void FrontTwinFilter::onSelectAll()
{
    __manipulateMap[qobject_cast<QWidget*>(sender())]->select();
}
