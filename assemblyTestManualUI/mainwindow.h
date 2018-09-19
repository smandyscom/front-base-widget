#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <controllermanualmode.h>
#include <adsclient.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    bool event(QEvent *event) Q_DECL_OVERRIDE;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;

    ControllerManualMode* m_controller;
};

#endif // MAINWINDOW_H
