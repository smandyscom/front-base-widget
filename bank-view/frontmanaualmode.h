#ifndef FRONTMANAUALMODE_H
#define FRONTMANAUALMODE_H

#include <QWidget>

namespace Ui {
class FrontManaualMode;
}

class FrontManaualMode : public QWidget
{
    Q_OBJECT

public:
    explicit FrontManaualMode(QWidget *parent = 0);
    ~FrontManaualMode();

private:
    Ui::FrontManaualMode *ui;
};

#endif // FRONTMANAUALMODE_H
