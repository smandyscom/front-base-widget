#ifndef FRONTMANAUALMODE_H
#define FRONTMANAUALMODE_H

#include <QWidget>
#include <basicblocksdefinition.h>

#include <modbuschannel.h>
#include <controllermanualmode.h>

#include <QVariant>
#include <QListView>
#include <QAbstractTableModel>

#include <junctionbankdatabase.h>

namespace Ui {
class FrontManaualMode;
}

class FrontManaualMode : public QWidget
{
    Q_OBJECT

public:
    explicit FrontManaualMode(QAbstractTableModel* wholeCommandBankModel,
                              QAbstractTableModel* wholeAxisBankModel,
                              QWidget *parent = 0);
    ~FrontManaualMode();
signals:
    //!
    //! \brief bankCoordinateSet
    //! \param coordinate
    //! Transmit parameters to Bank manager
    void bankCoordinateSet(float coordinate);
    //!
    //! \brief bankParameterSet
    //! \param parameters
    //! Transmit parameters to Bank manager
    void bankParameterSet(QVariant parameters);
    //!
    //! \brief onAxisChanged
    //! \param id
    //! Triggered by combobox
    void axisIdChanged(quint16 id);

public slots:
    //!
    //! \brief onCommandBlockChanged
    //! \param block
    //! Inject by bank manager
    void onCommandBlockChanged(ExtendedCommandBlock block);


protected slots:
    //!
    //! \brief onButtonBankSetClick
    //! Perform bank set related operations
    void onBankOperationPerformed();
    //!
    //! \brief onOperationPerform
    //!
    void onOperationPerform();
    //!
    //! \brief onButtonReleased
    //! Feed stop
    void onOperationStopped();

    //!
    //! \brief onFocusChanged
    //! \param old
    //! \param now
    //! Trigger by qApp
    void onFocusChanged(QWidget* old,QWidget* now);
    //!
    //! \brief onTimerTimeout
    //! Polling monitor status
    void onTimerTimeout();

    void onComboBoxIndexChanged();
protected:
    ExtendedCommandBlock __commandBlock;
    CommitBlock __commitOption;
    ControllerManualMode* controller;

    void setCommonParameters();
private:
    Ui::FrontManaualMode *ui;
    QTimer* updateTimer;

    //!
    //! \brief wholeCommandBank
    //!
    QAbstractTableModel* __wholeCommandBankModel;

};

#endif // FRONTMANAUALMODE_H
