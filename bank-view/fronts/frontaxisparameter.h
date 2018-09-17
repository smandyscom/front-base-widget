#ifndef FRONTAXISPARAMETER_H
#define FRONTAXISPARAMETER_H

#include <QWidget>
#include <QString>

#include <frontcommon.h>

#include <definitionmanualblock.h>
#include <definitionaxisblocks.h>
#include <definitioncommandblock.h>


namespace Ui {
class FrontAxisParameter;
}

class FrontAxisParameter :
        public FrontCommon
{
    Q_OBJECT

public:
    explicit FrontAxisParameter(QWidget *parent = 0);
    ~FrontAxisParameter();

    //! Commons
    bool showEvent(QShowEvent *event) Q_DECL_OVERRIDE;

protected slots:
    void onBankExecution();
    void onDirectExecution();
private:
    Ui::FrontAxisParameter *ui;
    //! Commons
    ManualModeDataBlock::Categrories m_categrory;
    int m_index;

    //!
    //! \brief m_monitorBlock
    //!
    AxisMonitorBlock m_monitorBlock;
    //!
    AxisOperationBlock m_operationBlock;
    ExtendedCommandBlock m_commandBlock;
    CellDataBlock* m_block;

    void setCommonParameters(PosCommandBlock& block);

    QMap<AxisMonitorBlock::OffsetMonitor,QLCDNumber*> m_lcdMap;

    void dynamicPropertyChanged(int key, QVariant value) Q_DECL_OVERRIDE;

};

#endif // FRONTAXISPARAMETER_H
