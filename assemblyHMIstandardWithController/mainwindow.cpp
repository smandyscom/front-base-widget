#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <loadinghelper.h>
#include <loadinghelpercontrollers.h>
#include <loadinghelperinterface.h>

#include <QDebug>

#include <qsqlrecord.h>

#include <widgetstatusbarbundle.h>

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
	QList<FrontSlot*> list = findChildren<FrontSlot*>();
	for (size_t i = 0; i < LoadingHelperControllers::m_controllersMaterial.count(); i++)
	{
		ControllerMaterialTransfer* ref = LoadingHelperControllers::m_controllersMaterial[i];
		LoadingHelperControllers::CrossLink(ref,list[i]);

		QSqlRecord record = utilities::getSqlTableSelectedRecord(LoadingHelperControllers::m_database->TableMap(QVariant::fromValue(JunctionBankDatabase::DEF_REGION)),
			QVariant::fromValue(HEADER_STRUCTURE::ID),
			i);

		list[i]->Setup(
			JunctionMaterialDatabase::Instance()->TableMap(ref->Index(),JunctionMaterialDatabase::MAT_DATA_SLOT),
			JunctionMaterialDatabase::Instance()->TableMap(ref->Index(), JunctionMaterialDatabase::MAT_HEADER_SLOT),
			record.value(QVariant::fromValue(HEADER_STRUCTURE::zh_TW).toString()).toString());
	}
	//!
	/*QTextBrowser* browser = new QTextBrowser();
	browser->setText("Browser");
	QLabel* initializingState = new QLabel();
	initializingState->setText("Label1");
	QLabel* currentMode = new QLabel();
	currentMode->setText("Label2");
	QList<QWidget*> list2{
		browser,
		initializingState,
		currentMode };
	for each (QWidget* var in list2)
	{
		ui->statusBar->addPermanentWidget(var);
	}*/
	WidgetStatusBarBundle* widget = new WidgetStatusBarBundle();
	ui->statusBar->addPermanentWidget(widget);
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