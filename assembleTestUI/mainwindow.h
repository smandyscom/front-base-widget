#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <adsclient.h>

#include <controllermainpanel.h>
#include <tempcontroller.h>

#include <commonhelper.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool event(QEvent *event) Q_DECL_OVERRIDE;
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_3_clicked(bool checked);

private:
    Ui::MainWindow *ui;
    ControllerMainPanel* m_controller;

    ControllerBase* __temp;
};

#endif // MAINWINDOW_H
