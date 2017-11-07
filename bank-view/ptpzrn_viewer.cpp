#include "ptpzrn_viewer.h"
#include "ui_ptpzrn_viewer.h"

#include <QSqlDatabase>
#include <QSqlRelationalTableModel>
#include <QSqlRelationalDelegate>
#include <QSqlRelation>
#include <QSqlQuery>
#include <QDir>
#include <QSqlError>
#include <QDataWidgetMapper>

#include <QDebug>
ptpzrn_viewer::ptpzrn_viewer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ptpzrn_viewer)
{
    ui->setupUi(this);

    //connect database
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("C:\\Users\\jlai10\\Github\\front-base-widget\\base.db");
    bool isOK = db.open();

    QString pt = QDir::currentPath();

    modelBank = new QSqlRelationalTableModel(this);
    modelBank->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);
    modelBank->setTable("PTP_BANK_WHOLE");
//    model->setRelation(2,QSqlRelation("DEF_IS_ABS","IS_ABS","DESCRIPTION")); //when relation activated, the update (submitAll()) would failed on VIEW ,
    // properbly happened on trigger

    //model->setHeaderData(0,Qt::Horizontal,tr("BANK_ID"));
//    model->setHeaderData(1,Qt::Horizontal,tr("AXIS_ID"));

//    model->setHeaderData(2,Qt::Horizontal,tr("IS_ABS"));
//    model->setHeaderData(3,Qt::Horizontal,tr("COORD"));
//    model->setHeaderData(4,Qt::Horizontal,tr("SPEED"));
//    model->setHeaderData(5,Qt::Horizontal,tr("ACC_TIME"));
//    model->setHeaderData(6,Qt::Horizontal,tr("DEC_TIME"));
//    model->setHeaderData(7,Qt::Horizontal,tr("TORQUE_LIMIT"));

    modelBank->setFilter("AXIS_ID='1'");
    isOK = modelBank->select();


    ui->tableView->setModel(modelBank);
    ui->tableView->hideColumn(1);
    ui->tableView->setItemDelegate(new QSqlRelationalDelegate(this)); //enable combo box on foreign key column

    //handling combobox - region
    QSqlTableModel* model2 = new QSqlTableModel(this);
    model2->setTable("REGION_BANK");
    model2->select();
    ui->comboBoxRegion->setModel(model2);
    ui->comboBoxRegion->setModelColumn(1); //show the human-readable column (NAME)

    QTableView* qtv = new QTableView(this);

    ui->comboBoxRegion->setView(qtv);
    qtv->hideColumn(0); //hide the column : REGION_ID

    connect(ui->comboBoxRegion,SIGNAL(currentIndexChanged(int)),this,SLOT(filterOutRegion()));

    QDataWidgetMapper* mapper = new QDataWidgetMapper(this);
    mapper->setModel(model2);
    mapper->addMapping(ui->comboBoxRegion,0,"currentData");

    //
    modelAxis = new QSqlTableModel(this);
    modelAxis->setTable("AXIS_BANK_ATTRIBUTES");
    modelAxis->select();

    ui->comboBoxAxisId->setModel(modelAxis);
    ui->comboBoxAxisId->setModelColumn(2);

    qtv=new QTableView(this);
    ui->comboBoxAxisId->setView(qtv);
    for(int i=0;i<modelAxis->columnCount();i++)
        qtv->hideColumn(i);
    qtv->showColumn(2);

    connect(ui->comboBoxAxisId,SIGNAL(currentIndexChanged(int)),this,SLOT(filterOutAxis()));
}

ptpzrn_viewer::~ptpzrn_viewer()
{
    delete ui;
}

void ptpzrn_viewer::filterOutRegion()
{
    qDebug().noquote() << ui->comboBoxRegion->currentIndex();
    qDebug().noquote() << ui->comboBoxRegion->currentData().toString();
    qDebug().noquote() << ui->comboBoxRegion->currentText();

    modelAxis->setFilter(tr("REGION='%1'").arg(ui->comboBoxRegion->currentIndex()+1));
    modelAxis->select();
}

void ptpzrn_viewer::filterOutAxis()
{
    modelBank->setFilter(tr("AXIS_ID='%1'").arg(ui->comboBoxAxisId->currentIndex()+1));
    modelBank->select();
}

void ptpzrn_viewer::on_buttonSubmit_clicked()
{
    modelBank->database().transaction();
    if (modelBank->submitAll()){
        modelBank->database().commit();
    }
    else{
        qDebug().noquote() << modelBank->data(modelBank->index(0,1)).toString();
        qDebug().noquote() << "Error Message";
        qDebug().noquote() << modelBank->lastError().text();
      qDebug().noquote() << modelBank->query().lastQuery();
        //ui->textBrowser->setText( model->lastError().text());
        modelBank->database().rollback();
        modelBank->revertAll();
        //this->setToolTip(model->database().lastError().text());
    }
}
