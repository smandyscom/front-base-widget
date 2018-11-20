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
	ui->menuVersion->setTitle(QString("build %1").arg(APP_VER));
	//
	QAction* exitAction = ui->menuBar->addAction(tr("Exit"));
	connect(exitAction, &QAction::triggered, this, &MainWindow::onClosing);
	//
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
	//! Slots cross link
	for (size_t i = 0; i < LoadingHelperControllers::m_controllersMaterial.count(); i++)
	{
		LoadingHelperControllers::CrossLink(LoadingHelperControllers::m_controllersMaterial[i],
			findChild<FrontSlot*>(QString("widgetSlot%1").arg(i+1)));
	}
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