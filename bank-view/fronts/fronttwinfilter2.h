#ifndef FRONTTWINFILTER2_H
#define FRONTTWINFILTER2_H

#include <QWidget>
#include <QComboBox>
#include <QSqlTableModel>

#include <utilities.h>
#include <abstractsqltableadpater.h>

namespace Ui {
class FrontTwinFilter2;
}

class FrontTwinFilter2 : public QWidget
{
    Q_OBJECT

public:
    explicit FrontTwinFilter2(QWidget *parent = 0);
    ~FrontTwinFilter2();

    void Setup(QSqlTableModel* dataTable,
               QVariant key1,
               QSqlTableModel* primaryTable,
               QVariant key2,
               QSqlTableModel* secondaryTable);


    //!
    //! \brief SelectedPrimaryId
    //! \return
    //! ID column
    int SelectedPrimaryId() const { return 0;}
    int SelectedSecondaryId() const {return 0;}
signals:
    void primarySelected(QVariant primaryKey);
    void secondarySelected(QVariant secondaryKey);
protected slots:
    void onSelectionChanged(int i);
    void onSelectAll();
private:
    Ui::FrontTwinFilter2 *ui;

    QSqlTableModel* __dataTable;
    QVariant __key1;
    //!
    //! \brief __primaryTable
    //! Load to
    QSqlTableModel* __primaryTable;
    QVariant __key2;
    //!
    //! \brief __secondaryTable
    //!
    QSqlTableModel* __secondaryTable;

    //!
    QMap<QWidget*,QSqlTableModel*> __manipulateMap;
};

#endif // FRONTTWINFILTER2_H
