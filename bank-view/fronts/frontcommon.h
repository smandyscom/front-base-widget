#ifndef FRONTCOMMON_H
#define FRONTCOMMON_H

#include <QtWidgets>
#include <QtEvents>

#include <commonhelper.h>
//#include <controllerbase.h>

class FrontCommon : public QWidget
{
    Q_OBJECT
public:
    //!
    //! \brief FrontCommon
    //! \param parent
    //! Defualt constructor , able to use "Promote" on QtUi designer
    explicit FrontCommon(QWidget *parent = nullptr);

    //!
    //! \brief event
    //! \param event
    //! \return
    //! Handling DynamicProperty
    bool event(QEvent* event) Q_DECL_OVERRIDE;

    void LinkController(QObject* controller);

signals:

public slots:
protected slots:
    void onCustomContextMenuShowed(const QPoint position);
    void onReloadQss();
protected:
    //!
    //! \brief m_controller
    //! Auto find-out linked controller
    QObject* m_controller;

    virtual void dynamicPropertyChanged(QString key,QVariant value){}
    virtual void dynamicPropertyChanged(int key,QVariant value){}
private:
    bool m_isQSSInitialized;
};

#endif // FRONTCOMMON_H
