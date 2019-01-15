#include "controllerauth.h"

ControllerAuth::ControllerAuth(QObject *parent) : QObject(parent)
{
    m_authTable = JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::DEF_AUTH);
}

void ControllerAuth::onAuthChangingRequired(AUTH::AuthRoles role,int password)
{
    QSqlRecord record = utilities::getSqlTableSelectedRecord(m_authTable,
                                                               QVariant::fromValue(HEADER_STRUCTURE::ID),
                                                               QVariant::fromValue(static_cast<int>(role)));

    int verification = record.value(QVariant::fromValue(AUTH::PASSWORD).toString()).toInt();

    //! verification <0 means no need to be verificated
    if(password==verification || verification < 0)
    {
		emit roleChanged(QVariant::fromValue(AUTH::PROP_AUTH),
			QVariant::fromValue(role));
    }
}
