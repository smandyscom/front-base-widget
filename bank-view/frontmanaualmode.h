#ifndef FRONTMANAUALMODE_H
#define FRONTMANAUALMODE_H

#include <QWidget>
#include <basicblocksdefinition.h>

#include <modbuschannel.h>
#include <controllermanualmode.h>

#include <QVariant>

namespace Ui {
class FrontManaualMode;
}

class FrontManaualMode : public QWidget
{
    Q_OBJECT

public:
    explicit FrontManaualMode(QWidget *parent = 0);
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
public slots:


protected slots:
    //!
    //! \brief onButtonBankSetClick
    //! Perform bank set related operations
    void onButtonBankSetClick();
    //!
    //! \brief onOperationPerform
    //!
    void onOperationPerform();
    //!
    //! \brief onButtonReleased
    //! Feed stop
    void onOperationStopped();
    //!
    //! \brief onAxisChanged
    //! \param id
    //! Triggered by combobox
    void onAxisChanged(quint16 id);
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
protected:
    GenericCommandBlock genericCommandBlock;
    ControllerManualMode* controller;

    void setCommonParameters();
private:
    Ui::FrontManaualMode *ui;
    QTimer* updateTimer;
};

#endif // FRONTMANAUALMODE_H
