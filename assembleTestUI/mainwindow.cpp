#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qRegisterMetaType<InterfaceRequest>();
    //!
//    JunctionBankDatabase::Instance();
    JunctionBankDatabase::Instance()->onInitialize();
//    InterfaceChannel::Instance();

//    __temp = new ControllerBase(0,0,100,this);
//    __temp->temp();
//    InterfaceChannel::Instance()->RegisterRoutines(0x100,100);
    AdsClient* __client = new AdsClient(AmsAddr(),true,0x4020,this);
    QList<InterfaceClient*> __clients = {__client};
    InterfaceChannel::Instance()->Clients(__clients);
    InterfaceChannel::Instance()->RegisterRoutines(256,QVariant::fromValue(CellDataBlock()),100);
//    auto __temp = new TempController(this);
    m_controller = new ControllerMainPanel(this);

    //!

    CommonHelper::setStyle("../style2.qss",this);
    qDebug() << this->styleSheet();

    __client->onTimerStart();
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
        auto __string = QString(static_cast<QDynamicPropertyChangeEvent*>(event)->propertyName());
        auto __value = this->property(__string.toStdString().c_str());

        this->style()->unpolish(ui->label); //valid
        this->style()->polish(ui->label);

        ui->statusBar->showMessage(QString("%1,%2").arg(__string).arg(__value.toString()));
        break;
    }
    default:
        break;
    }

    return QMainWindow::event(event);
}

void MainWindow::on_pushButton_clicked()
{
    setProperty("PROP",QVariant::fromValue(qrand()%3+1));
//    this->style()->unpolish(this);
//    this->style()->polish(this);


//    this->style()->unpolish(qApp);
//    this->style()->polish(qApp);

//    qApp->style()->unpolish(qApp);
//    qApp->style()->polish(qApp);


    qDebug() << property("PROP");
}

void MainWindow::on_pushButton_2_clicked()
{
    CommonHelper::setStyle("../style2.qss",this);
    qDebug() << this->styleSheet();
}

void MainWindow::on_pushButton_3_clicked()
{

    m_controller->setProperty(QVariant::fromValue(MainOperationBlock::BIT_1_TOGGLE_PAUSE).toString().toStdString().c_str(),true);
}
