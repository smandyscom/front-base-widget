#ifndef CONTROLLERAUTH_H
#define CONTROLLERAUTH_H

#include <QObject>

#include <QSqlTableModel>
#include <utilities.h>

#include <definitionauxiliarykeys.h>
#include <junctionbankdatabase.h>
//!
//! \brief The ControllerAuth class
//!
class ControllerAuth : public QObject
{
    Q_OBJECT
public:


    explicit ControllerAuth(QObject *parent = nullptr);


signals:
	void roleChanged(QVariant key, QVariant value);
public slots:
    void onAuthChangingRequired(AUTH::AuthRoles role, int password);
protected:
    QSqlTableModel* m_authTable;

};

#endif // CONTROLLERAUTH_H
