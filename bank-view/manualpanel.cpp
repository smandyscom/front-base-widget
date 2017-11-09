#include "manualpanel.h"
#include "ui_manualpanel.h"
#include <QModbusClient>
#include <QModbusTcpClient>
#include <QModbusReply>

#include <QUrl>
manualPanel::manualPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::manualPanel)
{
    ui->setupUi(this);

    //modbus link
    node = new QModbusTcpClient(this);

    //setup connections
    const QUrl url = QUrl::fromUserInput("127.0.0.1:502");
    node->setConnectionParameter(QModbusDevice::NetworkPortParameter, url.port());
    node->setConnectionParameter(QModbusDevice::NetworkAddressParameter,url.host());
    node->setTimeout(5000);
    node->setNumberOfRetries(10);
    node->connectDevice(); //success or not

    //QModbusReply rp;
    QModbusDataUnit du;
   //
    struct ptpBlock __ptp;
    for(int i=0;i<du.valueCount();i++)
        ((int*)(&__ptp))[i] = du.value(i);
}

manualPanel::~manualPanel()
{
    delete ui;
}


void manualPanel::on_buttonConnect_clicked()
{

}
