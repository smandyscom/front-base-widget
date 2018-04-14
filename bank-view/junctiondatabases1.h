#ifndef JUNCTIONDATABASES1_H
#define JUNCTIONDATABASES1_H

#include <QObject>
#include <junctionbankdatabase.h>

//!
//! \brief The JunctionDataBaseS1 class
//! Concrete table type
//!  S1 : memory shema1
class JunctionDataBaseS1 : public JunctionBankDatabase
{
    Q_OBJECT
public:


signals:

public slots:
    
protected:
    explicit JunctionDataBaseS1(QString databaseName,QObject *parent = nullptr);
     ~JunctionDatabaseS1();

    void decorateTable() Q_DECL_OVERRIDE;
};

#endif // JUNCTIONDATABASES1_H
