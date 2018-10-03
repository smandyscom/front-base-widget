#ifndef TABWIDGETOPERATIONMODE_H
#define TABWIDGETOPERATIONMODE_H

#include <QTabWidget>
#include <definitionmaincontrolblock.h>

class TabWidgetOperationMode : public QTabWidget
{
    Q_OBJECT
public:
    explicit TabWidgetOperationMode(QWidget *parent = nullptr);

    void Setup(QObject* controller);
signals:

public slots:
protected slots:
    void onCurrentWidgetChanged();
protected:
    QObject* m_controller;
};

#endif // TABWIDGETOPERATIONMODE_H
