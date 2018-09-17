#ifndef FRONTTWINFILTER2_H
#define FRONTTWINFILTER2_H

#include <QWidget>

namespace Ui {
class FrontTwinFilter2;
}

class FrontTwinFilter2 : public QWidget
{
    Q_OBJECT

public:
    explicit FrontTwinFilter2(QWidget *parent = 0);
    ~FrontTwinFilter2();

    //!
    //! \brief SelectedPrimaryId
    //! \return
    //! ID column
    int SelectedPrimaryId() const { return 0;}
    int SelectedSecondaryId() const {return 0;}

private:
    Ui::FrontTwinFilter2 *ui;
};

#endif // FRONTTWINFILTER2_H
