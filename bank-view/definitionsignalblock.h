#ifndef DEFINITIONSIGNALBLOCK_H
#define DEFINITIONSIGNALBLOCK_H

#include<QObject>

#include<definitionbasicblocks.h>
#include<definitionauxiliarykeys.h>

#include<abstractsqltableadpater.h>

class SignalMonitor :
        public AbstractDataBlock
{
public:
};
Q_DECLARE_METATYPE(SignalMonitor)

class SignalContext : public SignalMonitor
{
public:
    SignalContext() {}
};
Q_DECLARE_METATYPE(SignalContext)

namespace SignalBlock {

Q_NAMESPACE
enum DataBaseHeaders
{
    ID,
};
Q_ENUM_NS(DataBaseHeaders)


}

#endif // DEFINITIONSIGNALBLOCK_H
