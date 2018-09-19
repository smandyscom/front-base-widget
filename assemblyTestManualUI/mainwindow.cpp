#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qRegisterMetaType<InterfaceRequest>();

    AdsClient* m_client = new AdsClient(AmsAddr(),true,0x4020,this);
    QList<InterfaceClient*> m_clients = {m_client};
    InterfaceChannel::Instance()->Clients(m_clients);
    m_client->onTimerStart();

    m_controller = new ControllerManualMode(0,512,100,this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::DynamicPropertyChange:
    {
        QString key = QString(static_cast<QDynamicPropertyChangeEvent*>(event)->propertyName());
        QVariant value = property(key.toStdString().c_str());

        ui->statusBar->showMessage(QString("%1,%2").arg(key).arg(value.toString()));
        break;
    }
    default:
        break;
    }

    return QMainWindow::event(event);
}

void MainWindow::on_pushButton_clicked()
{
    m_controller->setProperty(QVariant::fromValue(ManualModeDataBlock::BIT_1_RUN).toString().toStdString().c_str(),true);
}
