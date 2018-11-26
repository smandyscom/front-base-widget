#ifndef FRONTSLOT_H
#define FRONTSLOT_H

#include <QWidget>

#include <definitionslotblock.h>
#include <utilities.h>
#include <definitionauxiliarykeys.h>

#include <QDialog>
#include <QTableView>
#include <QFormLayout>

#include <frontcommon.h>

namespace Ui {
class FrontSlot;
}


//!
// Update trigger source:
// By MaterialID/Valid property changed
// By Coresponding Model data changed
class FrontSlot : public FrontCommon
{
    Q_OBJECT

public:
    explicit FrontSlot(QWidget *parent = 0);
    ~FrontSlot();

	void Setup(QSqlTableModel* slot,
		QSqlTableModel* header,
		QString title);

protected slots:
	void onButtonClicked();
private:
    Ui::FrontSlot *ui;

    QDialog* m_dialog;
    QTableView* m_view;
	QSqlTableModel* m_model;

	//! Counters
	int m_totalCounter;
	int m_okCounter;
	int m_ngCounter;
	//! Used to check
	uint m_lastId;

	void dynamicPropertyChanged(int key, QVariant value) Q_DECL_OVERRIDE;
	
};

#endif // FRONTSLOT_H
