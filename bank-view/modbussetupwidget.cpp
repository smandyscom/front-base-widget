#include "modbussetupwidget.h"
#include "ui_modbussetupwidget.h"
#include <QMetaEnum>
#include <QUrl>
#include <QtSerialPort/QSerialPort>

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
    settingModel->setData(settingModel->index(7,1),QVariant(QUrl("127.0.0.1:502")),Qt::DecorationRole);
    //![0]



    ui->tableView->setModel(settingModel);

}

ModbusSetupWidget::~ModbusSetupWidget()
{
    delete ui;
}
