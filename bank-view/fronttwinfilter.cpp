#include "fronttwinfilter.h"
#include "ui_fronttwinfilter.h"

FrontTwinFilter::FrontTwinFilter(AbstractQVariantSqlTable *dataTable,
                                 AbstractQVariantSqlTable *primaryTable, AbstractQVariantSqlTable *regionTable,
                                 QString primaryKey, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrontTwinFilter),
    __dataTable(dataTable),
    __primaryTable(primaryTable),
    __regionTable(regionTable),
    __primaryKeyName(primaryKey)
{
    ui->setupUi(this);

    //!
    ui->comboBoxRegion->setModel(__regionTable);
    ui->comboBoxPrimary->setModel(__primaryTable);
    __manipulateMap[ui->comboBoxRegion] = __primaryTable;
    __manipulateMap[ui->comboBoxPrimary] = __dataTable;
    //__fieldNameMap[ui->comboBoxRegion] = QVariant::fromValue(REGION).value<QString>();
    __fieldNameMap[ui->comboBoxPrimary] = __primaryKeyName;

    __manipulateMap[ui->pushButtonPrimarySelectAll] = __dataTable;
    __manipulateMap[ui->pushButtonRegionSelectAll] = __primaryTable;
    //!
    QList<QComboBox*> __list = {ui->comboBoxRegion,
                                ui->comboBoxPrimary};
    foreach (QComboBox* var, __list) {
        AbstractQVariantSqlTable* __model = qobject_cast<AbstractQVariantSqlTable*>(var->model());
        //var->setModelColumn(__model->fieldIndex(QVariant::fromValue(NAME).value<QString>()));
        var->setView(new QTableView(var));
        connect(var,SIGNAL(currentIndexChanged(int)),this,SLOT(onSelectionChanged(int)));
    }
    //!
    QList<QPushButton*> __buttonList = {ui->pushButtonPrimarySelectAll,
                                        ui->pushButtonRegionSelectAll};
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
    QComboBox* __comboBox = qobject_cast<QComboBox*>(sender());
    AbstractQVariantSqlTable* __manipulateTable = __manipulateMap[qobject_cast<QWidget*>(sender())];
    QString fieldName =  __fieldNameMap[qobject_cast<QWidget*>(sender())];
    //TODO , should take id in model rather combox index
    int id = i;

    //!Set filter
    __manipulateTable->setFilter(tr("%1=%2").arg(fieldName).arg(id));
}
void FrontTwinFilter::onSelectAll()
{
    __manipulateMap[qobject_cast<QWidget*>(sender())]->select();
}
