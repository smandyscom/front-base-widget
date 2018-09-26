#ifndef FORM_H
#define FORM_H

#include <QWidget>

#include <frontcommon.h>

namespace Ui {
class Form;
}

class Form : public FrontCommon
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = nullptr);
    ~Form();

private:
    Ui::Form *ui;
};

#endif // FORM_H
