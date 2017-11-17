#ifndef MANUALPANEL_H
#define MANUALPANEL_H

#include <QWidget>
#include <QModbusClient>
#include <QtSerialPort/QSerialPort>
#include <bank-view_global.h>
namespace Ui {
class manualPanel;
}

struct rtuSettings {
    int parity = QSerialPort::EvenParity;
    int baud = QSerialPort::Baud19200;
    int dataBits = QSerialPort::Data8;
    int stopBits = QSerialPort::OneStop;
};
struct tcpSettings{
    QString port;
};

struct modbusClientSetting{
    union connectionSetting{
        rtuSettings __rtuSettings;
        tcpSettings __tcpSettings;
    };
    int responseTime = 1000;
    int numberOfRetries = 3;
    int type; //rtu or tcp
};

struct ptpBlock{
    qint16 axisId;
    quint16 controlWord;
    qint32 coordinate;
    qint32 speed;
    qint32 accelerationTime;
    qint32 decelerationTime;
    qint32 torqueLimit;
};
struct zrnBlock{
  struct ptpBlock base; //inherited from ptp block
  quint16 method;
  qint32 speedApproach;
  qint32 speedCreep;
};


//TODO
//a pivot to transform from/to different types
class ptpBlockObject : public QObject
{
    Q_OBJECT
public:
    ptpBlockObject() {}
    virtual ~ptpBlockObject() {}

    QModbusDataUnit dataUnit(void) const; //prepare the data unit
    //generate model record
};

class BANKVIEWSHARED_EXPORT manualPanel : public QWidget
{
    Q_OBJECT

public:
    explicit manualPanel(QWidget *parent = 0);
    ~manualPanel();

private slots:
    void on_buttonConnect_clicked();

private:
    Ui::manualPanel *ui;
    QModbusClient* node;
};

#endif // MANUALPANEL_H
