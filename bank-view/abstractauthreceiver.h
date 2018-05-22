#ifndef ABSTRACTAUTHRECEIVER_H
#define ABSTRACTAUTHRECEIVER_H

#include <QObject>
#include <QWidget>
#include <QMap>

#include <definitionauxiliarykeys.h>

class AbstractAuthReceiver
{
public:
    explicit AbstractAuthReceiver();

    virtual void onAuthSuccessChanged(AUTH::AuthRoles role);
protected:
    QMap<AUTH::AuthRoles,QList<QWidget*>> __disableList;

};

#endif // ABSTRACTAUTHRECEIVER_H
