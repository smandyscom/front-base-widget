#ifndef FRONTCOMMONMANUAL_H
#define FRONTCOMMONMANUAL_H

#include <QSqlTableModel>

#include <frontcommon.h>
#include <definitionmanualblock.h>

class FrontCommonManual :
        public FrontCommon
{
    Q_OBJECT
public:
    explicit FrontCommonManual(QWidget* parent=nullptr);

    //! Commons
    void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;
protected slots:
    void onMonitorIndexChanged();
protected:
    //! Commons
    ManualModeDataBlock::Categrories m_categrory;
    virtual int currentIndex() const;

    QSqlTableModel* mainDataTable;
    virtual QString currentFilter() const;
};

#endif // FRONTCOMMONMANUAL_H
