#include "modbussetupwidget.h"
#include "ui_modbussetupwidget.h"
#include <QMetaEnum>
#include <QUrl>
#include <QtSerialPort/QSerialPort>
#include <QItemEditorFactory>

#include <utilities.h>
#include <QDebug>

#include <QModbusTcpClient>
#include <QModbusTcpServer>
#include <QModbusRtuSerialMaster>
#include <QModbusRtuSerialSlave>

ModbusSetupWidget::ModbusSetupWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ModbusSetupWidget)
{
    ui->setupUi(this);


    //setting table initialization
    QMetaEnum parameterMetaEnum = QMetaEnum::fromType<QModbusDevice::ConnectionParameter>();

    settingModel = new QStandardItemModel(parameterMetaEnum.keyCount(),2,this);

    for (int i=0;i<parameterMetaEnum.keyCount();i++)
        settingModel->setData(settingModel->index(i,0),QString(parameterMetaEnum.key(i))); //set name

    //![0]
    //! register specific type/default value into field one-by-one
    //!
    settingModel->setData(settingModel->index(0,1),QVariant(QString("COM1")));
    settingModel->setData(settingModel->index(1,1),QVariant::fromValue(QSerialPort::NoParity));
    settingModel->setData(settingModel->index(2,1),QVariant::fromValue(QSerialPort::Baud38400));
    settingModel->setData(settingModel->index(3,1),QVariant::fromValue(QSerialPort::Data8));
    settingModel->setData(settingModel->index(4,1),QVariant::fromValue(QSerialPort::OneStop));
    settingModel->setData(settingModel->index(5,1),QVariant(QString("127.0.0.1")));
    settingModel->setData(settingModel->index(6,1),502);

    settingModel->setHeaderData(0,Qt::Horizontal,QVariant("Name"));
    settingModel->setHeaderData(1,Qt::Horizontal,QVariant("Value"));
    //![0]


    //![1]
    //! QItemEditorFactory registration
    //!
    QList<QPair<int,QItemEditorCreatorBase*>> lp;
    lp << utilities::generateEnumComboBoxEditor<QSerialPort::Parity>()
       << utilities::generateEnumComboBoxEditor<QSerialPort::BaudRate>()
       << utilities::generateEnumComboBoxEditor<QSerialPort::DataBits>()
       << utilities::generateEnumComboBoxEditor<QSerialPort::StopBits>();

    QPair<int,QItemEditorCreatorBase*> var;
    QItemEditorFactory* factory = new QItemEditorFactory();
    foreach (var, lp)
        factory->registerEditor(var.first,var.second);
    QItemEditorFactory::setDefaultFactory(factory);
    //![1]


    //![2]
    //! Prepare different kind of modbus device
    //! Issue instance and store into qmap
    //!
    deviceTorrent.insert(DeviceKinds::MASTER_CLIENT_TCP,new QModbusTcpClient(this));
    deviceTorrent.insert(DeviceKinds::SLAVE_SERVER_TCP,new QModbusTcpClient(this));
    deviceTorrent.insert(DeviceKinds::MASTER_CLIENT_RTUSERIAL,new QModbusRtuSerialMaster(this));
    deviceTorrent.insert(DeviceKinds::SLAVE_SERVER_RTUSERIAL,new QModbusRtuSerialSlave(this));

    deviceSelection = new EnumComboBoxGenericTemplate<DeviceKinds>(ui->widgetMode);

    ui->tableView->setModel(settingModel);

}

ModbusSetupWidget::~ModbusSetupWidget()
{
    delete ui;
}

void ModbusSetupWidget::on_buttonConnect_clicked()
{
    qDebug() << settingModel->data(settingModel->index(1,1)).typeName();
}
