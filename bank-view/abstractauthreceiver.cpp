#include "abstractauthreceiver.h"
#include <QDebug>

AbstractAuthReceiver::AbstractAuthReceiver()
{

}

void AbstractAuthReceiver::onAuthSuccessChanged(AUTH::AuthRoles role)
{
    qDebug() << QString("%1 logged").arg(QVariant::fromValue(role).toString());

    //! Open all widgets
    foreach (QList<QWidget*> __list, __disableList.values()) {
        foreach (QWidget* __widget, __list) {
            __widget->setEnabled(true);
        }
    }

    //! Shrink authorities
    foreach (AUTH::AuthRoles __compareRole, __disableList.keys()) {
        if(role <= __compareRole)
            foreach (QWidget* __widget, __disableList[__compareRole]) {
                __widget->setEnabled(false);
            }
    }

}
