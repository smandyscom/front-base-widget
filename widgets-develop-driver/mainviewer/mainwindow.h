#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QModbusDevice>

#include <QStandardItemModel>
namespace Ui {
class MainWindow;
}
 //Q_DECLARE_METATYPE(QModbusDevice::ConnectionParameter)

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private:
    Ui::MainWindow *ui;
    QStandardItemModel* model;
};


template<class T>
class EnumComboBox : public QComboBox
{
public:
    EnumComboBox(QWidget *parent = nullptr);
};

#endif // MAINWINDOW_H
