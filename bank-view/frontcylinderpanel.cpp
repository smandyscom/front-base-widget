#include "frontcylinderpanel.h"
#include "ui_frontcylinderpanel.h"

FrontCylinderPanel::FrontCylinderPanel(AbstractQVariantSqlTable *cylinderTable,
                                       QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrontCylinderPanel),
    __cylinderTable(cylinderTable)
{
    ui->setupUi(this);
    //!
    __inputFields = {
        TableModelCylinder::SEN_A_1,
        TableModelCylinder::SEN_A_2,
        TableModelCylinder::SEN_A_3,
        TableModelCylinder::SEN_A_4,
        TableModelCylinder::SEN_B_1,
        TableModelCylinder::SEN_B_2,
        TableModelCylinder::SEN_B_3,
        TableModelCylinder::SEN_B_4,
                    };
    __outputFields = {
        TableModelCylinder::ACT_A_1,
        TableModelCylinder::ACT_A_2,
        TableModelCylinder::ACT_B_1,
        TableModelCylinder::ACT_B_2,
    };
    //! Loading widgets
    new FrontSingleFilter(__cylinderTable,
                          JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::DEF_REGION),
                          "REGION",
                          "NAME",
                          ui->widgetFilter);
    new FrontBankTransfer(CommitBlock::SELECTION_CYLINDER,
                          ui->widgetBankTransfer);
    //!
    ui->tableViewInputs->setModel(JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::INPUT_ATTRIBUTES));
    ui->tableViewInputs->setModel(JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::OUTPUT_ATTRIBUTES));
    //!
    ui->tableViewCylinder->setModel(__cylinderTable);
    ui->tableViewCylinder->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableViewCylinder->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(ui->tableViewCylinder,SIGNAL(clicked(QModelIndex)),this,SLOT(onViewSelectionChanged()));
    //!
    //! \brief connect
    //!
    QList<QPushButton*> __buttonList = {ui->pushButtonGoA,
                                        ui->pushButtonGoB};
    connect(ui->pushButtonGoA,SIGNAL(clicked(bool)),this,SLOT(onClicked()));
}


FrontCylinderPanel::~FrontCylinderPanel()
{
    delete ui;
}

void FrontCylinderPanel::onClicked()
{
}

void FrontCylinderPanel::onViewSelectionChanged()
{
    QModelIndex __index = ui->tableViewCylinder->selectionModel()->selectedRows().first();
    QSqlRecord __record = __cylinderTable->record(__index.row());

    QList<QString> __inputAddress;
    QList<QString> __outputAddress;
    foreach (TableModelCylinder::Offset var, __inputFields) {
        __inputAddress.append(__record.value(QVariant::fromValue(var).toString()).toString());
    }
    foreach (TableModelCylinder::Offset var, __outputFields) {
        __outputAddress.append(__record.value(QVariant::fromValue(var).toString()).toString());
    }
    //! Generate filter string
    QString __inputFilterString = generateFilterString("PLC_ADDRESS",__inputAddress);
    QString __outputFilterString = generateFilterString("PLC_ADDRESS",__outputAddress);
    //filter out inputs
    qobject_cast<QSqlTableModel*>(ui->tableViewInputs->model())->setFilter(__inputFilterString);
    qobject_cast<QSqlTableModel*>(ui->tableViewOutputs->model())->setFilter(__outputFilterString);
}

QString FrontCylinderPanel::generateFilterString(QString key, QList<QString> conditions)
{
    int i=0;
    QString result;
    while (i<conditions.count()-1) {
        result.append(tr("%1=%2 OR ").arg(key).arg(conditions[i]));
        i++;
    }
    result.append(tr("%1=%2").arg(key).arg(conditions[conditions.count()-1]));

    return result;
}
