#ifndef FRONTCOMMON_H
#define FRONTCOMMON_H

#include <QtWidgets>
#include <QtEvents>

#include <controllerbase.h>

class FrontCommon : public QWidget
{
    Q_OBJECT
public:
    explicit FrontCommon(ControllerBase* controller,QWidget *parent = nullptr);

    //!
    //! \brief event
    //! \param event
    //! \return
    //! Handling DynamicProperty
    bool event(QEvent* event) Q_DECL_OVERRIDE;
signals:

public slots:
protected:
    ControllerBase* m_controller;
};

#endif // FRONTCOMMON_H
