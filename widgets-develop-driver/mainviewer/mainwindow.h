#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QModbusDevice>

#include <QStandardItemModel>
#include <QMetaEnum>
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


private slots:
    void on_pushButton_clicked();

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

class EnumComboBox2 : public QComboBox
{
    Q_OBJECT
    //you have to had property to output current setting
    Q_PROPERTY(QVariant value READ readValue WRITE setValue USER true)
public:
    QVariant readValue() const
    {
        return currentData();
    }
    void setValue(QVariant value)
    {
        setCurrentIndex(findData(value));
    }


    EnumComboBox2(QWidget *parent = nullptr) : QComboBox(parent) {
        QMetaEnum target = QMetaEnum::fromType<QModbusDevice::ConnectionParameter>();

        for(int i=0;i<target.keyCount();i++){
            insertItem(i
                       ,QString(target.key(i))
                       ,QVariant::fromValue(static_cast<QModbusDevice::ConnectionParameter>(target.value(i))));
        }

        __value = QVariant::fromValue(static_cast<QModbusDevice::ConnectionParameter>(0));

        setProperty("value",__value);
        QVariant v = property("value");
        int i =0;
    }

protected:
    QVariant __value;
};


#endif // MAINWINDOW_H
