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
    void filterOutRegion();
    void filterOutAxis();
    void on_buttonSubmit_clicked();

private:
    Ui::ptpzrn_viewer *ui;
    QSqlRelationalTableModel* modelBank;
    QSqlTableModel* modelAxis;
};

#endif // PTPZRN_VIEWER_H
