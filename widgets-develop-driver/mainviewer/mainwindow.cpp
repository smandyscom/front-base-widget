#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <ptpzrn_viewer.h>
#include <modbussetupwidget.h>
//#include <enumcombobox.h>
#include <QMetaEnum>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //ptpzrn_viewer* mw = new ptpzrn_viewer(this);

    ModbusSetupWidget* msw = new ModbusSetupWidget(ui->widgetModbus);

    QMetaEnum qme = QMetaEnum::fromType<QModbusDevice::ConnectionParameter>();
    EnumComboBox<QModbusDevice::ConnectionParameter>* ew = new EnumComboBox<QModbusDevice::ConnectionParameter>(ui->widgetComboBox);
        EnumComboBox<QModbusDevice::Error>* ew2 = new EnumComboBox<QModbusDevice::Error>(ui->widgetComboBox2);
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
