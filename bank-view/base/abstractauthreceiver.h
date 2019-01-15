#ifndef ABSTRACTAUTHRECEIVER_H
#define ABSTRACTAUTHRECEIVER_H

#include <QObject>
#include <QWidget>
#include <QMap>

#include <definitionauxiliarykeys.h>

class AbstractAuthReceiver : public QObject
{
	Q_OBJECT
public:
    explicit AbstractAuthReceiver(QObject* parent=nullptr);

    void onAuthSuccessChanged(AUTH::AuthRoles role);

	void addDisableWidget(AUTH::AuthRoles role, QWidget* widget);

protected:
    QMap<AUTH::AuthRoles,QList<QWidget*>> m_disableList;

};

#endif // ABSTRACTAUTHRECEIVER_H
