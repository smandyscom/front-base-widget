#ifndef CONTROLLERAUTH_H
#define CONTROLLERAUTH_H

#include <QObject>

#include <QSqlTableModel>
#include <utilities.h>

#include <definitionauxiliarykeys.h>
#include <abstractauthreceiver.h>
#include <junctionbankdatabase.h>

//!
//! \brief The ControllerAuth class
//!
class ControllerAuth : public QObject
{
    Q_OBJECT
public:


    explicit ControllerAuth(QObject *parent = nullptr);

    void linkAuthReceivers(QObject* headNode);

signals:
    void onRoleChanged(QString __roleName);
public slots:
    void onAuthChangingRequired(AUTH::AuthRoles role, int password);
protected:
    QSqlTableModel* __authTable;

    QList<AbstractAuthReceiver*> __authReceivers;
};

#endif // CONTROLLERAUTH_H
