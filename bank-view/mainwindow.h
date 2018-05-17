#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <modbuschannel.h>

#include <QModbusTcpClient>


#include <frontiooverride.h>
#include <frontconfigurationtransfer.h>
#include <frontcylinderpanel.h>

#include <frontmanaualmode.h>

#include <frontunitpanel.h>

#include <frontcontrolpanel.h>

#include <controllermaterialtransfer.h>


#include <QMap>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void onReadReply();
private:
    Ui::MainWindow *ui;

   QList<ControllerMaterialTransfer*> __materialSlots;

};

#endif // MAINWINDOW_H
