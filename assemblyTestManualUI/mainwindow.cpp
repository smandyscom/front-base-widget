#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qRegisterMetaType<InterfaceRequest>();

    AbstractDataBlock::Dimension = new QMap<DEF_BASIC_DIMENSION::Keys,qreal>();
    (*AbstractDataBlock::Dimension)[DEF_BASIC_DIMENSION::LENGTH] = 0.001;
    (*AbstractDataBlock::Dimension)[DEF_BASIC_DIMENSION::TIME] = 0.001;
    (*AbstractDataBlock::Dimension)[DEF_BASIC_DIMENSION::TORQUE_RATIO] = 0.001;

    AdsClient* m_client = new AdsClient(AmsAddr(),true,0x4020,this);
    QList<InterfaceClient*> m_clients = {m_client};
    InterfaceChannel::Instance()->Clients(m_clients);
    m_client->onTimerStart();

    JunctionBankDatabase::Instance()->onInitialize();
    ui->tab_2->Setup(JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::WHOLE_COMMAND_BLOCKS),
                     JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::WHOLE_AXIS),
                     JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::DEF_REGION),
                     JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::ERROR_CODE_AXIS),
                     JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::HEADER_COMMAND_BLOCKS));

    m_controller = new ControllerManualMode(0,512,100,ui->tab_2);

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
