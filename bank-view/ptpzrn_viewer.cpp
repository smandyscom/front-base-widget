#include "ptpzrn_viewer.h"
#include "ui_ptpzrn_viewer.h"

#include <QSqlDatabase>
#include <QSqlRelationalTableModel>
#include <QDir>
#include <QSqlError>
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
    model->setFilter("AXIS_ID='2'");
    isOK = model->select();


    ui->tableView->setModel(model);
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
        model->database().rollback();
        QString qs = model->database().lastError().text();
    }
}
