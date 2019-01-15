#include "abstractauthreceiver.h"
#include <QDebug>

AbstractAuthReceiver::AbstractAuthReceiver(QObject* parent) : QObject(parent)
{

}

void AbstractAuthReceiver::onAuthSuccessChanged(AUTH::AuthRoles role)
{
	qDebug() << QString("%1 logged").arg(QVariant::fromValue(role).toString());

    //! Open all widgets
    foreach (QList<QWidget*> m_list, m_disableList.values()) {
        foreach (QWidget* m_widget, m_list) {
            m_widget->setEnabled(true);
        }
    }

    //! Shrink authorities
    foreach (AUTH::AuthRoles m_compareRole, m_disableList.keys()) {
        if(role <= m_compareRole)
            foreach (QWidget* m_widget, m_disableList[m_compareRole]) {
                m_widget->setEnabled(false);
            }
    }
}

void AbstractAuthReceiver::addDisableWidget(AUTH::AuthRoles role, QWidget* widget)
{
	m_disableList[role].append(widget);
}