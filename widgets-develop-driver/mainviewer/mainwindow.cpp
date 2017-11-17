#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <ptpzrn_viewer.h>
#include <modbussetupwidget.h>
//#include <enumcombobox.h>
#include <QMetaEnum>
#include <QMetaObject>
#include <QItemEditorFactory>

#include <QModbusTcpServer>
#include <QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //ptpzrn_viewer* mw = new ptpzrn_viewer(this);

    ModbusSetupWidget* msw = new ModbusSetupWidget(ui->widgetModbus);

    //![1]

    QItemEditorFactory *factory = new QItemEditorFactory;

//    QItemEditorCreatorBase *editor =
//        new QStandardItemEditorCreator<EnumComboBox<QModbusDevice::ConnectionParameter>>();
        QItemEditorCreatorBase *editor =
            new QStandardItemEditorCreator<EnumComboBox2>();


    int id = qRegisterMetaType<QModbusDevice::ConnectionParameter>(); //run-time register , nessary
    id = QMetaType::type("QModbusDevice::ConnectionParameter");
    QString qs(QMetaType::typeName(id));
    id = QMetaType::type(qs.toLocal8Bit().data());


    factory->registerEditor(id, editor);

    //QItemEditorFactory::setDefaultFactory(factory);
    //![1]

    QMetaEnum qme = QMetaEnum::fromType<QModbusDevice::ConnectionParameter>();
    EnumComboBox<QModbusDevice::ConnectionParameter>* ew = new EnumComboBox<QModbusDevice::ConnectionParameter>(ui->widgetComboBox);
        EnumComboBox<QModbusDevice::Error>* ew2 = new EnumComboBox<QModbusDevice::Error>(ui->widgetComboBox2);


     //![2]
     //!
     //!Test if works
     //!
        //setting table initialization
        QMetaEnum parameterMetaEnum = QMetaEnum::fromType<QModbusDevice::ConnectionParameter>();
     model=   new QStandardItemModel(parameterMetaEnum.keyCount(),2,this);

            for (int i=0;i<parameterMetaEnum.keyCount();i++)
                model->setData(model->index(i,0),QString(parameterMetaEnum.key(i))); //set name

            model->setData(model->index(0,1),QVariant::fromValue(QModbusDevice::SerialBaudRateParameter));

            ui->tableView->setModel(model);
     //![2]

     //![3]
     QVariant var = 1;
     QModbusDevice::ConnectionParameter par = var.value<QModbusDevice::ConnectionParameter>();
     par = static_cast<QModbusDevice::ConnectionParameter>(2);

     var = QVariant::fromValue(QModbusDevice::ConnectedState);
     QString a = QVariant::fromValue(QModbusDevice::ConnectedState).value<QString>();
     //qDebug() << a;
     //qDebug() << QVariant(5).value<QModbusDevice::ConnectionParameter>();
     qDebug() << QVariant("QModbusDevice::SerialBaudRateParameter").value<QModbusDevice::ConnectionParameter>();
     qDebug() << QVariant("QModbusDevice::SerialDataBitsParameter").value<QModbusDevice::ConnectionParameter>();
     qDebug() << QVariant("QModbusDevice::NetworkAddressParameter").value<QModbusDevice::ConnectionParameter>();
     qDebug() << QVariant("NetworkAddressParameter").value<QModbusDevice::ConnectionParameter>();

     QModbusDevice* qmd =  new QModbusTcpServer(this);
     qmd->setConnectionParameter(QModbusDevice::SerialPortNameParameter,"COM1");
     //![3]
}

MainWindow::~MainWindow()
{
    delete ui;
}

template<class T>
EnumComboBox<T>::EnumComboBox(QWidget *parent ):QComboBox(parent){
    //object:
    // 1. load all items from the definition of enum , including its key/value
    // 2. when user selected specific item


    QMetaEnum target = QMetaEnum::fromType<T>();

    for(int i=0;i<target.keyCount();i++)
        insertItem(i,QString(target.key(i)),target.value(i));
}

void MainWindow::on_pushButton_clicked()
{
    qDebug() << model->data(model->index(0,1)).typeName();
}
