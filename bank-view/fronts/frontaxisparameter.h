#ifndef FRONTAXISPARAMETER_H
#define FRONTAXISPARAMETER_H

#include <QWidget>
#include <QString>

#include <frontcommonmanual.h>

#include <abstractsqltableadpater.h>
#include <definitionmanualblock.h>
#include <definitionaxisblocks.h>
#include <definitioncommandblock.h>


namespace Ui {
class FrontAxisParameter;
}

class FrontAxisParameter :
        public FrontCommonManual
{
    Q_OBJECT

public:
    explicit FrontAxisParameter(QWidget *parent = 0);
    ~FrontAxisParameter();

    //!
    //! \brief Setup
    //! \param commandBlockTable
    //! \param axisTable
    //! \param regionTable
    //! \param axisErrorTable
    //!
    void Setup(QSqlTableModel* commandBlockTable,
               QSqlTableModel* axisTable,
               QSqlTableModel* regionTable,
               QSqlTableModel* axisErrorTable,
               QSqlTableModel* commandBlockTableHeader);


protected slots:
    void onBankExecution();
    void onDirectExecution(bool value);
    void onInterrupted(bool value);
private:
    Ui::FrontAxisParameter *ui;
    //!
    AbstractSqlTableAdpater* m_commblockAdaptor;
    QSqlTableModel* m_axisTable;
    QSqlTableModel* m_axisErrorTable;
    //! \brief m_monitorBlock
    //!
    AxisMonitorBlock m_monitorBlock;
    QList<QVariant> m_monitorOperation;
    QList<QVariant> m_runStatus;

    void setCommonParameters(PosCommandBlock& block);
    int selectedCommandBlockIndex() const;
    QVariant selectedAxisValue(int axisId,QVariant key) const;

    QMap<AxisMonitorBlock::OffsetMonitor,QLCDNumber*> m_lcdMap;

    void dynamicPropertyChanged(int key, QVariant value) Q_DECL_OVERRIDE;

    int currentIndex() const Q_DECL_OVERRIDE;
    QString currentFilter() const Q_DECL_OVERRIDE;
};

#endif // FRONTAXISPARAMETER_H
