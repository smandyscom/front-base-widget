#include "loadinghelperinterface.h"

LoadingHelperInterface::LoadingHelperInterface(QObject *parent)
	: QObject(parent)
{
}

LoadingHelperInterface::~LoadingHelperInterface()
{
}

void LoadingHelperInterface::LoadingInterfaceV1()
{
  QSqlTableModel* table = 
	  JunctionBankDatabase::Instance()->TableMap(JunctionBankDatabase::CLIENTS_CONFIG);

  //table->setFilter(nullptr);
  //table->select();

  QList<InterfaceClient*> list;
  for (size_t i = 0; i < table->rowCount(); i++)
  {
	   list << AdsClient::genClient(table->record(i));
  }

  InterfaceChannel::Instance()->Clients(list);
}
