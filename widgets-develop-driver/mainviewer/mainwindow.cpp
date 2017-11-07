#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <ptpzrn_viewer.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ptpzrn_viewer* mw = new ptpzrn_viewer(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
