#ifndef TABWIDGETOPERATIONMODE_H
#define TABWIDGETOPERATIONMODE_H

#include <QTabWidget>
#include <definitionmaincontrolblock.h>

#include <propertyportcommon.h>

class TabWidgetOperationMode : public QTabWidget
{
    Q_OBJECT
public:
    explicit TabWidgetOperationMode(QWidget *parent = nullptr);

    //void Setup(QObject* controller);


	QObject* m_controller;
signals:

public slots:
protected slots:
    void onCurrentWidgetChanged();
protected:
    
};

#endif // TABWIDGETOPERATIONMODE_H
