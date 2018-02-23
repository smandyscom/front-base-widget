#ifndef CONTROLLERMANUALMODE_H
#define CONTROLLERMANUALMODE_H

#include <QObject>
#include <QStateMachine>

#include <baselayerdefinitions.h>
using namespace BaseLayer;


//!
//! \brief The ControllerManualMode class
//! The state machine about to control manual mode
//! Inherit QStateMachine
class ControllerManualMode : public QObject
{
    Q_OBJECT
public:
    enum ManualContext
    {
        ENGAGED_HMI=0x00000000,
        DONE=0x00010000,
        AXIS_ADR=0x00000001,
        POS_COMMAND=0x00000002,
        POS_FEEDBACK=0x00000004,
        SPD_FEEDBACK=0x00000006,
        TOR_FEEDBACK=0x00000008,
        ENGAGED_PLC=0x00000010,
        RUN=0x00010010,
        CANCEL=0x00020010,
        MODE=0x00000011,
        COMMIT_CONTENT=0x00000012,
        COMMIT_INDEX=0x00000013,
        BLOCK_HEAD=0x00000020,
    }
    Q_ENUM(ManualContext)

    explicit ControllerManualMode(QObject *parent = nullptr);

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
public slots:

    //onFocused , start monitoring
    // light on HMI_ONLINE
    //not on Focused , off HMI_ONLINE
protected slots:
    //!
    //! \brief onStateEntered
    //! Doing reading
    void onStateEntered();
protected:
    //!
    //! \brief trunsactionController
    //! Control the handshaking sequence
    QStateMachine* trunsactionController;

    //!
    //! \brief blockCache
    //! Mind alignment problem
    QVariant blockCache;
};

#endif // CONTROLLERMANUALMODE_H
