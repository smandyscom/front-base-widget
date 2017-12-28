#ifndef BASELAYERDEFINITIONS_H
#define BASELAYERDEFINITIONS_H

#include <QObject>

//address : channel,type,holding register address(16bits)
//according register address to update cluster

//address-QVariant pair

namespace BaseLayer {

struct ModbusDriverAddress{
    quint32 reserved;
    quint8 channelAddress;
    quint8 registerType;
    quint16 registerAddress;
};

//other driver addressing mode
struct DummyDriverAddress{
  quint64 reserved;
};

}
Q_DECLARE_METATYPE(BaseLayer::ModbusDriverAddress)
Q_DECLARE_METATYPE(BaseLayer::DummyDriverAddress)

#endif // BASELAYERDEFINITIONS_H
