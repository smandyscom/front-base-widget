#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <QDebug>
#include <qregularexpression.h>


#include <qsqlrecord.h>
#include <messagerecorder.h>

#include <widgetstatusbarbundle.h>


#define THREADED true


MainWindow::MainWindow(QWidget *parent) :
    MainWindowCommon(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	
	MessageRecorder::Instance();//initiating log
	//search all action
	QList<QAction*> actions = findChildren<QAction*>();
	for each (QAction* var in actions)
	{
		connect(var, &QAction::triggered, this, &MainWindow::onDownloadUploadActionTrigger);
	}	
	//
	QAction* exitAction = ui->menuBar->addAction(tr("Exit"));
	connect(exitAction, &QAction::triggered, this, &MainWindow::onClosing);
	//
	m_thread = new QThread();
	m_thread->setObjectName("workerThread");
	
	

	LoadingHelperInterface* m_interface = new LoadingHelperInterface();
#ifdef DEBUG

#endif // DEBUG
#ifdef THREADED
	m_interface->moveToThread(m_thread);
	connect(m_thread, &QThread::started, m_interface, &LoadingHelperInterface::LoadingInterfaceV1);
#else
	m_interface->LoadingInterfaceV1();
#endif // THREADED

    
    //! Load controllers
	m_controllers = new LoadingHelperControllers();
	m_controllers->LoadAdaptors(); // load in main thread

	//! Load UI/Model
	LoadingHelper::CombineModelViewV1(ui->tabAxis,
		ui->tabCylinder,
		ui->tabUnit,
		nullptr,
		ui->widgetMain,
		ui->tabIO,
		ui->tabSafety,
		ui->tabSignal);

#ifdef THREADED
	m_controllers->moveToThread(m_thread);
	connect(m_thread, &QThread::started, m_controllers, &LoadingHelperControllers::ControllersLoadingRoutineV1);
	connect(m_controllers, &LoadingHelperControllers::controllerLoaded, this, &MainWindow::onControllerLoaded);
	
	qRegisterMetaType<QVector<int>>();
	qRegisterMetaType<Qt::Orientation>();

	m_thread->start();
#else
	m_controllers->ControllersLoadingRoutineV1();
	onControllerLoaded();
	m_controllers->m_controllerTransfer->onStateMachineStart();
#endif // THREADED

	//!Auth	
	m_auth = new ControllerAuth(this);
	ui->actionOperator->setProperty(QVariant::fromValue(AUTH::PROP_AUTH).toString().toStdString().c_str(),QVariant::fromValue(AUTH::ROLE_OPERATOR));
	ui->actionEngineer->setProperty(QVariant::fromValue(AUTH::PROP_AUTH).toString().toStdString().c_str(), QVariant::fromValue(AUTH::ROLE_ENGINEER));
	ui->actionDeveloper->setProperty(QVariant::fromValue(AUTH::PROP_AUTH).toString().toStdString().c_str(), QVariant::fromValue(AUTH::ROLE_DEVELOPER));
	actions.clear();
	actions << ui->actionOperator
		<< ui->actionEngineer
		<< ui->actionDeveloper;
	for each (QAction* var in actions)
	{
		connect(var, &QAction::triggered,this,&MainWindow::onAuthAction);
	}
	for each (FrontCommon* var in findChildren<FrontCommon*>())
	{
		connect(m_auth, &ControllerAuth::roleChanged, static_cast<PropertyPortCommon*>(var->port()), &PropertyPortCommon::onPropertyChanged);
	}
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onClosing()
{
	close(); //trigger form to close
}


void MainWindow::onControllerLoaded()
{
	
 //! Link controller and fronts
    QList<FrontCommon*> frontList{ui->tabAxis,
                ui->tabCylinder,
                ui->tabUnit,
				ui->tabSignal};
    foreach (FrontCommon* var, frontList) {
		m_controllers->CrossLink(m_controllers->m_controllerTransfer->port(),var->port());
    }
	//!
	m_controllers->CrossLink(m_controllers->m_controllerMain->port(),ui->widgetMain->port());
	m_controllers->CrossLink(m_controllers->m_controllerTransfer->port(), ui->widgetMain->port());
    //!
	m_controllers->CrossLink(ui->tabWidget->m_controller, m_controllers->m_controllerMain->port());
	//! Slots cross link
	QList<FrontSlot*> list = findChildren<FrontSlot*>();
	for (int i = 0; i < m_controllers->m_controllersMaterial.count(); i++)
	{
		ControllerMaterialTransfer* ref = m_controllers->m_controllersMaterial[i];
		m_controllers->CrossLink(ref->port(),list[i]->port());

		QSqlRecord record = utilities::getSqlTableSelectedRecord(m_controllers->m_database->TableMap(QVariant::fromValue(JunctionBankDatabase::DEF_REGION)),
			QVariant::fromValue(HEADER_STRUCTURE::ID),
			ref->Index());

		list[i]->Setup(
			JunctionMaterialDatabase::Instance()->TableMap(ref->Index(),JunctionMaterialDatabase::MAT_DATA_SLOT),
			JunctionMaterialDatabase::Instance()->TableMap(ref->Index(), JunctionMaterialDatabase::MAT_HEADER_SLOT),
			record.value(QVariant::fromValue(HEADER_STRUCTURE::zh_TW).toString()).toString());
	}

	
	WidgetStatusBarBundle* widget = new WidgetStatusBarBundle();
	QString name = widget->objectName();
	ui->statusBar->addPermanentWidget(widget);
	
	m_controllers->CrossLink(m_controllers->m_controllerTransfer->port(), widget->port());
	m_controllers->CrossLink(m_controllers->m_controllerMain->port(), widget->port());

	QDateTime timeDate = QDateTime::fromString(QString(APP_VER), "ddd MM\/dd\/yyyy, h:mm:ss.z");
	qDebug() << timeDate.isValid();
	qDebug() << timeDate.toString("ddMMyyhhmmss");
	widget->setProperty(QString::number(WidgetStatusBarBundle::MODEL).toStdString().c_str(),
		QString("%1\n%2").arg(
		utilities::getSqlTableSelectedRecord(m_controllers->m_database->TableMap(JunctionBankDatabase::PROPERTIES),
			QVariant::fromValue(HEADER_STRUCTURE::NAME),
			QVariant::fromValue(WidgetStatusBarBundle::MODEL)).value("VALUE").toString())
	.arg(timeDate.toString("ddMMyyhhmmss")));
	
	//!
	showFullScreen();
	//!
	m_controllers->m_controllerTransfer->onStateMachineStart();
}

void MainWindow::onDownloadUploadActionTrigger()
{
	//!Resolve action name
	auto name = sender()->objectName();

	QRegularExpression re("action([Read|Write]*)(\\w+)");
	QRegularExpressionMatch match = re.match(name);
	if (match.hasMatch())
	{
		QString mode = match.captured(1);
		QString value = match.captured(2);

		mode = (QString("BATCH_ALL_%1_MODE").arg(mode)).toUpper();
		value = (QString("SELECTION_%1").arg(value)).toUpper();

		ManualModeDataBlock::TransferCommand cmd =
			QVariant::fromValue(mode).value<ManualModeDataBlock::TransferCommand>();

		ManualModeDataBlock::Categrories cat = 
			QVariant::fromValue(value).value<ManualModeDataBlock::Categrories>();
	
		//!
		emit qobject_cast<PropertyPortCommon*>(m_controllers->m_controllerTransfer->port())
			->internalPropertyChange(QVariant::fromValue(cmd),
				QVariant::fromValue(cat));
	}	
}

void MainWindow::onAuthAction()
{
	m_auth->onAuthChangingRequired(sender()->property(QVariant::fromValue(AUTH::PROP_AUTH).toString().toStdString().c_str()).value<AUTH::AuthRoles>(),
		QInputDialog::getInt(this,
			tr("輸入密碼"),
			tr("密碼")));
}