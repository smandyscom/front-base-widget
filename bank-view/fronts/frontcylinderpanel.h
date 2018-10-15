#ifndef FRONTCYLINDERPANEL_H
#define FRONTCYLINDERPANEL_H

#include <frontcommonmanual.h>
#include <abstractsqltableadpater.h>
#include <definitionmanualblock.h>

#include <definitioncylinderblock.h>
#include <definitionioattributes.h>
#include <definitionauxiliarykeys.h>

//#include <abstractauthreceiver.h>

namespace Ui {
class FrontCylinderPanel;
}

using namespace CylinderBlock;
using namespace IoAttributes;

class FrontCylinderPanel :
        public FrontCommonManual
{
    Q_OBJECT

public:
    explicit FrontCylinderPanel(QWidget *parent = nullptr);
    ~FrontCylinderPanel();

    void Setup(QSqlTableModel* cylinderTable,
               QSqlTableModel* regionTable,
               QSqlTableModel* cylinderTableHeader);

protected slots:
    void onCylinderCommandClicked();
private:
    Ui::FrontCylinderPanel *ui;

    CylinderMonitorBlock m_monitorBlock;
    QList<QVariant> m_status;

    void dynamicPropertyChanged(int key, QVariant value) Q_DECL_OVERRIDE;

    int m_index;
	int currentIndex()  Q_DECL_OVERRIDE;
    QString currentFilter() const Q_DECL_OVERRIDE;
};

#endif // FRONTCYLINDERPANEL_H
