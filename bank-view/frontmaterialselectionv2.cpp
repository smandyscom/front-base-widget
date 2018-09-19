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
    __referenceModel->setTable(QVariant::fromValue(VIEW_MAT_TABLE_HOUSING).toString());
    __referenceModel->select();

    //!
    QList<QueryKeys> __list = {
        HOUSING1,
        HOUSING2
    };

    foreach (QVariant var, __list) {
        DelegateMaterialSelector* __delegate =
                new DelegateMaterialSelector(__referenceModel,VALUE,ui->tableViewReportHead);
        ui->tableViewReportHead->setItemDelegateForRow(var.toInt(),
                                                       __delegate);
    }
    ui->tableViewReportHead->setItemDelegateForRow(REPORTDATE,new DelegateDateSelector(VALUE)); //date editor

    //!
    QSqlTableModel* __header = new QSqlTableModel(this,db);
    __header->setTable(QVariant::fromValue(HEADER_REPORT_HEAD).toString());
    __header->select();
    HEADER_STRUCTURE::HeaderRender::renderViewHeader(__header,ui->tableViewReportHead);

    //!
    connect(ui->pushButtonReport,SIGNAL(clicked(bool)),this,SLOT(onReportGenerating()));

    //!
    __dialog = new QDialog(this);

    QGridLayout* gridLayout = new QGridLayout(__dialog);
    __report = new FrontReportQueryTable(db,__dialog);
    __dialog->setLayout(gridLayout);
    gridLayout->addWidget(__report);
    __dialog->setWindowState(Qt::WindowMaximized);
}

FrontMaterialSelectionV2::~FrontMaterialSelectionV2()
{
    delete ui;
}

void FrontMaterialSelectionV2::onReportGenerating()
{
    __dialog->exec();
}
