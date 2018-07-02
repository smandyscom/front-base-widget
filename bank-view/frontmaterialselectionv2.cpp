#include "frontmaterialselectionv2.h"
#include "ui_frontmaterialselectionv2.h"

FrontMaterialSelectionV2::FrontMaterialSelectionV2(QSqlDatabase db, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrontMaterialSelectionV2)
{
    ui->setupUi(this);
    //!
    QSqlTableModel* __model = new QSqlTableModel(this,db);
    __model->setTable(QVariant::fromValue(REPORT_HEAD).toString());
    __model->setEditStrategy(QSqlTableModel::OnFieldChange);
    __model->select();

    ui->tableViewReportHead->setModel(__model);

    QSqlTableModel* __referenceModel =  new QSqlTableModel(this,db);
    __referenceModel->setTable(QVariant::fromValue(VIEW_MAT_TABLE_KEY).toString());
    __referenceModel->select();



    foreach (QVariant var, utilities::listupEnumVariant<TableRows>()) {
        DelegateMaterialSelector* __delegate =
                new DelegateMaterialSelector(__referenceModel,VALUE,ui->tableViewReportHead);
        ui->tableViewReportHead->setItemDelegateForRow(var.toInt(),
                                                       __delegate);
    }
    ui->tableViewReportHead->setItemDelegateForRow(7,new DelegateDateSelector(VALUE));

    //!
    QSqlTableModel* __header = new QSqlTableModel(this,db);
    __header->setTable(QVariant::fromValue(HEADER_REPORT_HEAD).toString());
    __header->select();
    HEADER_STRUCTURE::HeaderRender::renderViewHeader(__header,ui->tableViewReportHead);
}

FrontMaterialSelectionV2::~FrontMaterialSelectionV2()
{
    delete ui;
}
