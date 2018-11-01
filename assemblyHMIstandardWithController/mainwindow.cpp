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
	connect(ui->actionExit, &QAction::triggered, this, &MainWindow::onClosing);
   
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

void MainWindow::onClosing()
{
	close(); //trigger form to close
}