#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <mainwindowcommon.h>



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
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
