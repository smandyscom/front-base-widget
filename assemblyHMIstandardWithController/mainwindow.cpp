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
		ui->tabSafety,
		ui->tabSignal);
    //! Load controllers
    LoadingHelperControllers::ControllersLoadingRoutineV1();
    //! Link controller and fronts
    QList<FrontCommon*> frontList{ui->tabAxis,
                ui->tabCylinder,
                ui->tabUnit,
                ui->tabConfiguration,
				ui->tabSignal};
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
	QString name = widget->objectName();
	ui->statusBar->addPermanentWidget(widget);
	
	LoadingHelperControllers::CrossLink(LoadingHelperControllers::m_controllerTransfer, widget);
	LoadingHelperControllers::CrossLink(LoadingHelperControllers::m_controllerMain, widget);
	/*widget->setProperty(QVariant::fromValue(WidgetStatusBarBundle::MODEL).toString().toStdString().c_str(),
		utilities::getSqlTableSelectedRecord(LoadingHelperControllers::m_database->TableMap(JunctionBankDatabase::PROPERTIES),
			QVariant::fromValue(HEADER_STRUCTURE::NAME),
			QVariant::fromValue(WidgetStatusBarBundle::MODEL)).value("VALUE"));
	
	QVariant var = QVariant::fromValue(DEF_REGION::NAME);
	QVariant var2 = QVariant::fromValue(SlotBlock::OK);*/
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