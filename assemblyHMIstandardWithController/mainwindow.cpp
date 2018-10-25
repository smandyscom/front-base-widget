#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <loadinghelper.h>
#include <loadinghelpercontrollers.h>
#include <loadinghelperinterface.h>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    MainWindowCommon(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //! Interface initialization
 //   AmsAddr addr{{5,60,134,238,1,1},
 //               851};
 //   InterfaceClient* mainClient = new AdsClient(addr);
 //   InterfaceClient* inputClient1 = new AdsClient(addr,false,AdsClient::PLCADS_RWI,517960);
	////InterfaceClient* inputClient2 = new AdsClient(addr, false, AdsClient::PLCADS_RWI, 586928);
 //   InterfaceClient* outputClient1 = new AdsClient(addr,false,AdsClient::PLCADS_RWQ, 513417);
	////InterfaceClient* outputClient2 = new AdsClient(addr, false, AdsClient::PLCADS_RWQ, 634002);

 //   InterfaceChannel::Instance()->Clients(QList<InterfaceClient*>{
	//	mainClient,
	//		inputClient1,
	//		//inputClient2,
	//		outputClient1,
	//		//outputClient2
	//});
	LoadingHelper::LoadBaseDatabase();
	LoadingHelperInterface::LoadingInterfaceV1();


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
	//!
	showFullScreen();
}

MainWindow::~MainWindow()
{
    delete ui;
}
