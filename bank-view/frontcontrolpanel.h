#ifndef FRONTCONTROLPANEL_H
#define FRONTCONTROLPANEL_H

#include <QWidget>

namespace Ui {
class frontControlPanel;
}

class frontControlPanel : public QWidget
{
    Q_OBJECT

public:
    explicit frontControlPanel(QWidget *parent = 0);
    ~frontControlPanel();

private:
    Ui::frontControlPanel *ui;
};

#endif // FRONTCONTROLPANEL_H
