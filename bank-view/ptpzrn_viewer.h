#ifndef PTPZRN_VIEWER_H
#define PTPZRN_VIEWER_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlRelationalTableModel>

#include <bank-view_global.h>

namespace Ui {
class ptpzrn_viewer;
}

class BANKVIEWSHARED_EXPORT ptpzrn_viewer : public QWidget
{
    Q_OBJECT

public:
    explicit ptpzrn_viewer(QWidget *parent = 0);
    ~ptpzrn_viewer();

private slots:
    void submitClicked();

    void on_pushButton_clicked();

private:
    Ui::ptpzrn_viewer *ui;
    QSqlRelationalTableModel* model;
};

#endif // PTPZRN_VIEWER_H
