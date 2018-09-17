#include "fronttwinfilter2.h"
#include "ui_fronttwinfilter2.h"

FrontTwinFilter2::FrontTwinFilter2(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrontTwinFilter2)
{
    ui->setupUi(this);

}

void FrontTwinFilter2::Setup(QSqlTableModel *dataTable, QVariant key1, QSqlTableModel *primaryTable, QVariant key2, QSqlTableModel *secondaryTable)
{
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
        var->clear();
        var->setModel(__manipulateMap[var]);
        QListView* qtv = new QListView(var);
        var->setView(qtv);
        //! Hide all but NAME
        int __nameColumnIndex = __manipulateMap[var]->fieldIndex("zh_TW");

        connect(var,SIGNAL(currentIndexChanged(int)),this,SLOT(onSelectionChanged(int)));
        var->setModelColumn(__nameColumnIndex);
        var->setCurrentIndex(0);
    }
    //!
    QList<QPushButton*> __buttonList = {ui->pushButtonPrimarySelectAll,
                                        ui->pushButtonSecondary};
    foreach (QPushButton* var, __buttonList) {
        connect(var,&QPushButton::clicked,this,&FrontTwinFilter2::onSelectAll);
    }
}

FrontTwinFilter2::~FrontTwinFilter2()
{
    delete ui;
}

void FrontTwinFilter2::onSelectionChanged(int i)
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
void FrontTwinFilter2::onSelectAll()
{
    __manipulateMap[qobject_cast<QWidget*>(sender())]->setFilter(nullptr);
}
