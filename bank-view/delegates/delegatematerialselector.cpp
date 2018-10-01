#include "delegatematerialselector.h"

DelegateMaterialSelector::DelegateMaterialSelector(QSqlTableModel* reference, int interestedColumn, QObject *parent) :
    QStyledItemDelegate(parent),
    __interestedColumn(interestedColumn),
    __referenceModel(reference)
{

}


QWidget* DelegateMaterialSelector::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.column()==__interestedColumn)
    {
        QComboBox* __editor = new QComboBox(parent);
        __referenceModel->select();//reselect
        utilities::linkQComboBoxAndModel(__editor,__referenceModel,QVariant::fromValue(HEADER_STRUCTURE::ID));

        return __editor;
    }
    else
        return QStyledItemDelegate::createEditor(parent,option,index);
}

//!
//! \brief DelegateMaterialSelector::setEditorData
//! \param editor
//! \param index
//! Read data from model to editor
void DelegateMaterialSelector::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    //! ComboBox would retain last selected id
    QComboBox* __combo = qobject_cast<QComboBox*>(editor);
    __combo->setCurrentIndex(index.row());
}

void DelegateMaterialSelector::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox* __combo = qobject_cast<QComboBox*>(editor);

    model->setData(index,
                   __referenceModel->record(__combo->currentIndex()).value(QVariant::fromValue(HEADER_STRUCTURE::ID).toString()));
    //__selectedIndex = __combo->currentIndex();
}

void DelegateMaterialSelector::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}
