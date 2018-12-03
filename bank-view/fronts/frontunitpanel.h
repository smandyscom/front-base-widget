#ifndef FRONTUNITPANEL_H
#define FRONTUNITPANEL_H

#include <frontcommonmanual.h>
#include <definitionmanualblock.h>
#include <definitionunitblocks.h>
#include <utilities.h>

//#include <abstractauthreceiver.h>

namespace Ui {
class FrontUnitPanel;
}

class FrontUnitPanel :
        public FrontCommonManual
//        public AbstractAuthReceiver
{
    Q_OBJECT

public:
    explicit FrontUnitPanel(QWidget *parent = 0);
    ~FrontUnitPanel();

    void Setup(QSqlTableModel* unitTable, 
		QSqlTableModel *unitTableHeader,
		QSqlTableModel* unitConfig,
		QSqlTableModel* unitConfigHeader);

protected slots:
    void onCommandClick();
protected:
    Ui::FrontUnitPanel *ui;

    QMap<QPushButton*,UnitOperationBlock::ControlBits> m_controlMap;//Widget,Address

    UnitMonitorBlock m_monitorBlock;
    QList<QVariant> m_status;
    QList<QVariant> m_condition;

    void dynamicPropertyChanged(int key, QVariant value) Q_DECL_OVERRIDE;

    //!Override
    int m_index;
	int currentIndex() Q_DECL_OVERRIDE;
};

#endif // FRONTUNITPANEL_H
