#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

#include <loadinghelper.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    LoadingHelper::CombineModelViewV1(ui->tabAxis,
                                      ui->tabCylinder,
                                      ui->tabUnit,
                                      ui->tabConfiguration,
                                      ui->tabMain,
                                      ui->tabIO);
}

MainWindow::~MainWindow()
{
    delete ui;
}
