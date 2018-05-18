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



protected slots:
    void onReadReply();
protected:
    Ui::MainWindow *ui;

    void closeEvent(QCloseEvent* event) Q_DECL_OVERRIDE;
    QList<ControllerMaterialTransfer*> __materialSlots;

    bool __isClosing;
};

#endif // MAINWINDOW_H
