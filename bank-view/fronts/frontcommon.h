#ifndef FRONTCOMMON_H
#define FRONTCOMMON_H

#include <QtWidgets>
#include <QtEvents>

class FrontCommon : public QWidget
{
    Q_OBJECT
public:
    explicit FrontCommon(QWidget *parent = nullptr);

    bool event(QEvent* event) Q_DECL_OVERRIDE;
signals:

public slots:
};

#endif // FRONTCOMMON_H