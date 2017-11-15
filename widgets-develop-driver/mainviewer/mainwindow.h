#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};


template<class T>
class EnumComboBox : public QComboBox
{
public:
    EnumComboBox(QWidget *parent = nullptr);
};

#endif // MAINWINDOW_H
