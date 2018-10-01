#ifndef DELEGATEMATERIALSELECTOR_H
#define DELEGATEMATERIALSELECTOR_H

#include <QObject>
#include <QStyledItemDelegate>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QComboBox>

#include <utilities.h>
#include <definitionauxiliarykeys.h>

//!
//! \brief The DelegateMaterialSelector class
//! Generic item selector refer to referenceModel
class DelegateMaterialSelector : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit DelegateMaterialSelector(QSqlTableModel* reference,int interestedColumn=0,QObject *parent = nullptr);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const Q_DECL_OVERRIDE;

    void setEditorData(QWidget *editor, const QModelIndex &index) const Q_DECL_OVERRIDE;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const Q_DECL_OVERRIDE;

    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;

protected:
    QSqlTableModel* __referenceModel;
    int __interestedColumn;
    //int __selectedIndex;
};

#endif // DELEGATEMATERIALSELECTOR_H
