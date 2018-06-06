#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <modbuschannel.h>

#include <QModbusTcpClient>
#include <QInputDialog>

#include <frontiooverride.h>
#include <frontconfigurationtransfer.h>
#include <frontcylinderpanel.h>
#include <frontmanaualmode.h>
#include <frontunitpanel.h>
#include <frontcontrolpanel.h>

#include <controllermaterialtransfer.h>
#include <controllerauth.h>
#include <abstractauthreceiver.h>

#include <QMap>

typedef QPair<int,int> ChannelSlot;

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
    void onAuthTriggered();
    void onAuthChanged(QString roleName);
protected:
    Ui::MainWindow *ui;

    void closeEvent(QCloseEvent* event) Q_DECL_OVERRIDE;
    QList<ControllerMaterialTransfer *> __materialSlots;

    bool __isClosing;

    ControllerAuth* __authController;
    QMap<QObject*,AUTH::AuthRoles> __authRoleTable;
};

#endif // MAINWINDOW_H
