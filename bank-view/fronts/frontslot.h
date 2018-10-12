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

class FrontSlot : public FrontCommon
{
    Q_OBJECT

public:
    explicit FrontSlot(QWidget *parent = 0);
    ~FrontSlot();

	void Setup(QSqlTableModel* slot,
		QSqlTableModel* header);

protected:
	void onButtonClicked();
	void onDataChanged(const QModelIndex &topLeft,
		const QModelIndex &bottomRight,
		const QVector<int> &roles = QVector<int>());
private:
    Ui::FrontSlot *ui;

    //ControllerMaterialTransfer* __controller;
    QDialog* m_dialog;
    QTableView* m_view;

	//! Counters
	int m_totalCounter;
	int m_okCounter;
	int m_ngCounter;
	uint m_lastId;
	uint m_currentId;

	void update();

	void dynamicPropertyChanged(int key, QVariant value) Q_DECL_OVERRIDE;
};

#endif // FRONTSLOT_H
