#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);



    //! Initialize Modbus Channels

    //! Intialize Database
    JunctionBankDatabase::DatabaseName("base.db");
    JunctionBankDatabase::Instance()->onInitialize();

    //! Initialize FrontManaul panel
    FrontManaualMode* fmm = new FrontManaualMode(JunctionBankDatabase::Instance()->CommandBlockTable(),
                                                 JunctionBankDatabase::Instance()->AxisTable(),
                                                 ui->tabManual);
}

MainWindow::~MainWindow()
{
    delete ui;
}


