#include "ptpzrn_viewer.h"
#include "ui_ptpzrn_viewer.h"

#include <QSqlDatabase>
#include <QSqlRelationalTableModel>
#include <QSqlRelationalDelegate>
#include <QSqlRelation>
#include <QSqlQuery>
#include <QDir>
#include <QSqlError>

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

    model = new QSqlRelationalTableModel(this);
    model->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);
    model->setTable("PTP_BANK_WHOLE");
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

    model->setFilter("AXIS_ID='1'");
    isOK = model->select();


    ui->tableView->setModel(model);
    ui->tableView->hideColumn(1);
    ui->tableView->setItemDelegate(new QSqlRelationalDelegate(this)); //enable combo box on foreign key column
}

ptpzrn_viewer::~ptpzrn_viewer()
{
    delete ui;
}

void ptpzrn_viewer::submitClicked()
{
    model->database().transaction();
    if (!model->database().commit()){

    }
    else{
        model->database().rollback();
    }

}

void ptpzrn_viewer::on_pushButton_clicked()
{
    model->database().transaction();
    if (model->submitAll()){
        model->database().commit();
    }
    else{
        qDebug().noquote() << model->data(model->index(0,1)).toString();
        qDebug().noquote() << "Error Message";
        qDebug().noquote() << model->lastError().text();
      qDebug().noquote() << model->query().lastQuery();
        //ui->textBrowser->setText( model->lastError().text());
        model->database().rollback();
        model->revertAll();
        //this->setToolTip(model->database().lastError().text());
    }
}
