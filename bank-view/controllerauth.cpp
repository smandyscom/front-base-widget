#include "controllerauth.h"

ControllerAuth::ControllerAuth(QObject *parent) : QObject(parent)
{
    __authTable = JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::DEF_AUTH);
}

void ControllerAuth::onAuthChangingRequired(AUTH::AuthRoles role,int password)
{
    QSqlRecord __record = utilities::getSqlTableSelectedRecord(__authTable,
                                                               QVariant::fromValue(HEADER_STRUCTURE::ID),
                                                               QVariant::fromValue(static_cast<int>(role)));
    int verification = __record.value("Password").toInt();

    //! verification <0 means no need to be verificated
    if(password==verification || verification < 0)
    {
        foreach (AbstractAuthReceiver* __receiver, __authReceivers) {
            __receiver->onAuthSuccessChanged(role);
        }

        emit onRoleChanged(__record.value(utilities::trimNamespace(QVariant::fromValue(HEADER_STRUCTURE::zh_TW))).toString());
    }
}

void ControllerAuth::linkAuthReceivers(QObject *headNode)
{
   AbstractAuthReceiver* target =  dynamic_cast<AbstractAuthReceiver*>(headNode);
   if(target!=nullptr)
       __authReceivers.append(target);

   //! Recursive
   if(headNode->children().count() >0)
   {
       foreach (QObject* node, headNode->children()) {
           linkAuthReceivers(node);
       }
   }
   else
       return;
}
