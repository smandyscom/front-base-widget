#ifndef FRONTAXISPARAMETER_H
#define FRONTAXISPARAMETER_H

#include <QWidget>

namespace Ui {
class FrontAxisParameter;
}

class FrontAxisParameter : public QWidget
{
    Q_OBJECT

public:
    explicit FrontAxisParameter(QWidget *parent = 0);
    ~FrontAxisParameter();

private:
    Ui::FrontAxisParameter *ui;
};

#endif // FRONTAXISPARAMETER_H
