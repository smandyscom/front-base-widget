#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //! Interface initialization
    InterfaceClient* simuClient = new AdsClient(AmsAddr(),true);
    InterfaceChannel::Instance()->Clients(QList<InterfaceClient*>{simuClient});
    //! Load UI/Model
    LoadingHelper::CombineModelViewV1(ui->tabAxis,
                                      ui->tabCylinder,
                                      ui->tabUnit,
                                      ui->tabConfiguration,
                                      ui->widgetMain,
                                      ui->tabIO);
    //! Load controllers
    LoadingHelperControllers::ControllersLoadingRoutineV1();
    //! Link controller and fronts
    QList<FrontCommon*> frontList{ui->tabAxis,
                ui->tabCylinder,
                ui->tabUnit,
                ui->tabConfiguration};
    foreach (FrontCommon* var, frontList) {
        LoadingHelper::CrossLink(LoadingHelperControllers::m_controllerTransfer,var);
    }
    LoadingHelper::CrossLink(LoadingHelperControllers::m_controllerMain,ui->widgetMain);
}

MainWindow::~MainWindow()
{
    delete ui;
}
