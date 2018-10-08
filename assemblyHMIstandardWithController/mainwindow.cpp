#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //! Interface initialization
    AmsAddr addr{{5,60,134,238,1,1},
                851};
    InterfaceClient* mainClient = new AdsClient(addr);
    InterfaceClient* inputClient = new AdsClient(addr,false,AdsClient::PLCADS_RWI,525231);
    InterfaceClient* outputClient = new AdsClient(addr,false,AdsClient::PLCADS_RWQ,520917);

    InterfaceChannel::Instance()->Clients(QList<InterfaceClient*>{
                                              mainClient,
                                              inputClient,
                                              outputClient
                                          });
    //! Load UI/Model
    LoadingHelper::CombineModelViewV1(ui->tabAxis,
                                      ui->tabCylinder,
                                      ui->tabUnit,
                                      ui->tabConfiguration,
                                      ui->widgetMain,
                                      ui->tabIO,
		ui->tabSafety);
    //! Load controllers
    LoadingHelperControllers::ControllersLoadingRoutineV1();
    //! Link controller and fronts
    QList<FrontCommon*> frontList{ui->tabAxis,
                ui->tabCylinder,
                ui->tabUnit,
                ui->tabConfiguration};
    foreach (FrontCommon* var, frontList) {
        LoadingHelperControllers::CrossLink(LoadingHelperControllers::m_controllerTransfer,var);
    }
    LoadingHelperControllers::CrossLink(LoadingHelperControllers::m_controllerMain,ui->widgetMain);
    //!
    ui->tabWidget->Setup(LoadingHelperControllers::m_controllerMain);
}

MainWindow::~MainWindow()
{
    delete ui;
}
