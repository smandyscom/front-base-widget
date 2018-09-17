#ifndef FRONTCOMMON_H
#define FRONTCOMMON_H

#include <QtWidgets>
#include <QtEvents>

#include <controllerbase.h>

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
signals:

public slots:
protected:
    //!
    //! \brief m_controller
    //! Auto find-out linked controller
    ControllerBase* m_controller;

    virtual void dynamicPropertyChanged(QString key,QVariant value){}
    virtual void dynamicPropertyChanged(int key,QVariant value){}
};

#endif // FRONTCOMMON_H
