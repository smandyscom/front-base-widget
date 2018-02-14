#include "modbuschannel.h"
#include <utilities.h>
#include <QCoreApplication>

ModbusChannel::ModbusChannel(QObject *parent) :
    QObject(parent)
{
//    if(driver != nullptr){
//        channelGateWays = new ModbusSerializedClient(driver,
//                                                    serverAddress,
//                                                    this);
//    }
//    else{
//        channelGateWays = nullptr; //no gateway, simulation only
//    }

    preparedReadRequest.setRegisterType(QModbusDataUnit::HoldingRegisters);
    preparedWriteRequest.setRegisterType(QModbusDataUnit::HoldingRegisters);
}

void ModbusChannel::beginUpdate(ModbusDriverAddress address)
{
    // From map query type of QVariant
    // Get the object size , so that you can make right request
    QVariant variant = dataMap[address];
    size_t sizeInWord = (utilities::sizeOf(variant))/2; //size of in byte

    preparedReadRequest.setStartAddress(address.getRegisterAddress());
    preparedReadRequest.setValueCount(sizeInWord);
    channelGateWays[address.getChannel()]->pushRequest(new ModbusSerializedClient::ModbusRequest(preparedReadRequest,ModbusSerializedClient::READ));
}

void ModbusChannel::commit(ModbusDriverAddress address, const QVariant value)
{
    writeData(address,value.data());
    //
    // From map query type of QVariant
    // Get the object size , so that you can make right request
    QVariant variant = dataMap[address];
    size_t sizeInWord = (utilities::sizeOf(variant))/2; //size of in byte

    preparedWriteRequest.setStartAddress(address.getRegisterAddress());
    preparedWriteRequest.setValueCount(sizeInWord);
    for(int i=0;i<sizeInWord;i++)
        preparedWriteRequest.setValue(i,reinterpret_cast<quint16*>(value.data())[i]);
    channelGateWays[address.getChannel()]->pushRequest(new ModbusSerializedClient::ModbusRequest(preparedWriteRequest,ModbusSerializedClient::WRITE));
}

QVariant ModbusChannel::update(const ModbusDriverAddress& modbusAddress)
{
    return dataMap[modbusAddress];
}

quint16* ModbusChannel::toCacheAddress(const ModbusDriverAddress& modbusAddress)
{
    return &(channelCache[modbusAddress.getChannel()])[modbusAddress.getRegisterAddress()];
}

//!
//! \brief ModbusChannel::onReplyUpdated
//! \param result
//!
void ModbusChannel::onUpdated(QModbusDataUnit result)
{
   //identify which channel?
   int channelIndex = channelGateWays.indexOf(qobject_cast<ModbusSerializedClient*>(sender()));
   //recover the address
   ModbusDriverAddress modbusAddress;
   modbusAddress.setChannel(channelIndex);
   modbusAddress.setRegisterAddress(result.startAddress());
   //write
   writeData(modbusAddress,result.values().data());

   emit raiseUpdateEvent(new UpdateEvent(modbusAddress,dataMap[modbusAddress]));
}

//!
//! \brief ModbusChannel::writeData
//! \param modbusAddress
//! \param source
//!
void ModbusChannel::writeData(ModbusDriverAddress modbusAddress, const void* source)
{
    //write into cache
    memcpy(toCacheAddress(modbusAddress),
           source,
           utilities::sizeOf(dataMap[modbusAddress]));
    //update variant
    // anchor the variant reference to cache?? is that possible?
    memcpy(dataMap[modbusAddress.getRegisterAddress()].data(),
           toCacheAddress(modbusAddress),
           utilities::sizeOf(dataMap[modbusAddress]));
}
