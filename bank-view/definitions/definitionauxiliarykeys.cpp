#include<definitionauxiliarykeys.h>


//!
//! \brief HEADER_STRUCTURE::HeaderRender::renderViewHeader
//! \param reference
//! \param view
//! \param key
//! Show those key-defined column only
void HEADER_STRUCTURE::HeaderRender::renderViewHeader(QSqlTableModel *reference,
                                                      QTableView *view,
                                                      Headers key,
                                                      bool isStrech)
{
	reference->setFilter(nullptr);
	reference->select();
    //! Hide all
    for(int i=0;i<view->model()->columnCount();i++)
        view->hideColumn(i);
    //! Show those showed in reference
    for(int i=0;i<reference->rowCount();i++)
    {
       QSqlRecord __record = reference->record(i);
        view->showColumn(__record.value(QVariant::fromValue(ID).toString()).toInt());
        view->model()->setHeaderData(__record.value(QVariant::fromValue(ID).toString()).toInt(),
                                     Qt::Horizontal,
                                     __record.value(QVariant::fromValue(key).toString()));
    }
    if(isStrech)
        view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}
