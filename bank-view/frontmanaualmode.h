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
public slots:
    void onButtonClick();
    void onButtonPressed();
    //!
    //! \brief onButtonReleased
    //! Feed stop
    void onButtonReleased();
    //!
    //! \brief onAxisChanged
    //! \param id
    //!
    void onAxisChanged(quint16 id);

protected:
    GenericCommandBlock genericCommandBlock;
    ControllerManualMode* controller;

private:
    Ui::FrontManaualMode *ui;
};

#endif // FRONTMANAUALMODE_H
