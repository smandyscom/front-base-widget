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
               QSqlTableModel* keyTable1,
               QVariant key2,
               QSqlTableModel* keyTable2);


    //!
    //! \brief SelectedPrimaryId
    //! \return
    //! ID column
    int SelectedKey1() const { return m_value1.toInt();}
    int SelectedKey2() const {return m_value2.toInt();}

    QString Filter1() const {return m_filter1;}
    QString Filter2() const {return m_filter2;}

signals:
    void key1Selected(QVariant primaryKey);
    void key2Selected(QVariant secondaryKey);
protected slots:
    void onSelectionChanged(int i);
    void onSelectAll();
private:
    Ui::FrontTwinFilter2 *ui;
    //!
    //! \brief __dataTable
    //! The table to be manipulation
    QSqlTableModel* m_dataTable;
    QVariant m_key1;
    QVariant m_value1;
    QString m_filter1;
    //!
    //! \brief __primaryTable
    //! Load to
    QSqlTableModel* m_keyTable1;
    QVariant m_key2;
    QVariant m_value2;
    QString m_filter2;
    //!
    //! \brief __secondaryTable
    //!
    QSqlTableModel* m_keyTable2;

    //!
    QMap<QWidget*,QSqlTableModel*> m_manipulateMap;
};

#endif // FRONTTWINFILTER2_H
