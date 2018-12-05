#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <mainwindowcommon.h>
#include <version.h>
#include <qthread.h>

#include<propertyportcommon.h>

#include <loadinghelper.h>
#include <loadinghelpercontrollers.h>
#include <loadinghelperinterface.h>


namespace Ui {
class MainWindow;
}

class MainWindow : public MainWindowCommon
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected slots:
	void onClosing();
	void onControllerLoaded();
private:
    Ui::MainWindow *ui;

	QThread* m_thread;
	LoadingHelperControllers* m_controllers;
};

#endif // MAINWINDOW_H
