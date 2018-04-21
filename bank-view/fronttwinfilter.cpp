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
    __key1(key1),
    __primaryTable(primaryTable),
    __key2(key2),
    __secondaryTable(secondaryTable)
{
    ui->setupUi(this);

    //!
    ui->comboBoxSecondary->setModel(__secondaryTable);
    ui->comboBoxPrimary->setModel(__primaryTable);
    __manipulateMap[ui->pushButtonPrimarySelectAll] = __dataTable;
    __manipulateMap[ui->pushButtonSecondary] = __primaryTable;
    __manipulateMap[ui->comboBoxPrimary] = __primaryTable;
    __manipulateMap[ui->comboBoxSecondary] = __secondaryTable;
    //!
    QList<QComboBox*> __list = {ui->comboBoxPrimary,
                                ui->comboBoxSecondary};
    foreach (QComboBox* var, __list) {
        var->setModel(__manipulateMap[var]);
        QTableView* qtv = new QTableView(var);
        var->setView(qtv);
        connect(var,SIGNAL(currentIndexChanged(int)),this,SLOT(onSelectionChanged(int)));
        var->setModelColumn(1);
        var->setCurrentIndex(0);
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
        value = __primaryTable->record(i).value(FIRST_COLUMN);
        __dataTable->setFilter(utilities::generateFilterString(__key1,value));

        emit primarySelected(value);
    }
    else if(sender() == ui->comboBoxSecondary)
    {
        value = __secondaryTable->record(i).value(FIRST_COLUMN);
        __primaryTable->setFilter(utilities::generateFilterString(__key2,
                                                                  value));
        emit secondarySelected(value);
    }
}
void FrontTwinFilter::onSelectAll()
{
    __manipulateMap[qobject_cast<QWidget*>(sender())]->setFilter(nullptr);
}
