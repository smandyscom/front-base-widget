#ifndef CONTROLLERMANUALMODE_H
#define CONTROLLERMANUALMODE_H

#include <QObject>
#include <QStateMachine>

#include <baselayerdefinitions.h>
using namespace BaseLayer;

#include <modbuschannel.h>
#include <basicblocksdefinition.h>
#include <QVariant>

typedef quint16 CommitIndex ;

enum CommitSelection : quint16
{
    AXIS = 0,
    CYLINDER = 1,
    COMMAND_BLOCK = 2,
};
enum CommitMode : quint16
{
    BLOCK_COMMAND=0,
    DOWNLOAD = 15, //PLC<-HMI
    UPLOAD = 16,   //PLC->HMI
};
struct CommitBlock
{
    CommitMode mode;
    CommitSelection selection;
    CommitIndex index;
};
Q_DECLARE_METATYPE(CommitBlock)
//Q_ENUM_NS(CommitContentDefinition)
//Q_ENUM_NS(ExecutionMode)

//!
//! \brief The ControllerManualMode class
//! The state machine about to control manual mode
//! Inherit QStateMachine
class ControllerManualMode : public QStateMachine
{
    Q_OBJECT
public:
    enum ManualContext
    {
        STATUS_WORD=0x00000000,
        ENGAGED_PLC=0x00000000,
        DONE=0x00010000,
        AXIS_ADR=0x00000001,
        POS_COMMAND=0x00000002,
        POS_FEEDBACK=0x00000004,
        SPD_FEEDBACK=0x00000006,
        TOR_FEEDBACK=0x00000008,
        ENGAGED_HMI=0x00000010,
        RUN=0x00010010,
        CANCEL=0x00020010,
        COMMIT_BLOCK=0x00000011,
        COMMIT_MODE=0x00000011,
        COMMIT_SELECTION=0x00000012,
        COMMIT_INDEX=0x00000013,
        DATA_BLOCK_HEAD=0x00000020,
        DATA_BLOCK_END=0x7F,
    };

    Q_ENUM(ManualContext)

    explicit ControllerManualMode(QObject *parent = nullptr);

    //!
    //! Data interfaces
    void setCommitMode(CommitMode mode);
    void setCommitSelection(CommitSelection selection);
    void setCommitIndex(CommitIndex index);
    //!
    //! \brief blockCache
    //! Mind alignment problem
    QVariant dataBlockCache;

signals:
    //!
    //! \brief readWordIn
    //! \param address
    //! \param wordData
    //!
    void requireReadData(AbstractAddress address,const QVariant data);
    //!
    //! \brief writeData
    //! \param address
    //! \param data
    //!
    void requireWriteData(AbstractAddress address,const QVariant data);

    void triggerOperation();
public slots:

    void onFocusEntered();
    void onFocusExited();
    //onFocused , start monitoring
    // light on HMI_ONLINE
    //not on Focused , off HMI_ONLINE

protected:

    FullCommandBlock dummyFcb;
    CommitBlock commitBlockCache;

    ModbusChannel* channel; //TODO
};

#endif // CONTROLLERMANUALMODE_H
