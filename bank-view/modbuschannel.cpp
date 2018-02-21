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
    for(int i=0;i<CHANNEL_NUM;i++)
        channelCache.append(new quint16[USHRT_MAX]);

    int sz = channelCache.size();
    sz=channelGateWays.size();

    preparedReadRequest.setRegisterType(QModbusDataUnit::HoldingRegisters);
    preparedWriteRequest.setRegisterType(QModbusDataUnit::HoldingRegisters);
}

void ModbusChannel::beginUpdate(ModbusDriverAddress address, const QVariant dataForm)
{
    // From map query type of QVariant
    // Get the object size , so that you can make right request
    size_t sizeInWord = (utilities::sizeOf(dataForm))/2; //size of in byte

    preparedReadRequest.setStartAddress(address.getRegisterAddress());
    preparedReadRequest.setValueCount(sizeInWord);

    //once no specific channel , do no action
    if(address.getChannel() >= channelGateWays.size())
        return;
    channelGateWays[address.getChannel()]->pushRequest(new ModbusSerializedClient::ModbusRequest(preparedReadRequest,ModbusSerializedClient::READ));
}

void ModbusChannel::commit(ModbusDriverAddress address, const QVariant value)
{
    writeData(address,value.data(),utilities::sizeOf(value));
    //
    // From map query type of QVariant
    // Get the object size , so that you can make right request
    size_t sizeInWord = (utilities::sizeOf(value))/2; //size of in byte

    preparedWriteRequest.setStartAddress(address.getRegisterAddress());
    preparedWriteRequest.setValueCount(sizeInWord);
    for(int i=0;i<sizeInWord;i++)
        preparedWriteRequest.setValue(i,reinterpret_cast<const quint16*>(value.data())[i]);

    //once no specific channel , do no action
    if(address.getChannel() >= channelGateWays.size() )
        return;
    channelGateWays[address.getChannel()]->pushRequest(new ModbusSerializedClient::ModbusRequest(preparedWriteRequest,ModbusSerializedClient::WRITE));
}

void ModbusChannel::update(const ModbusDriverAddress modbusAddress, QVariant &fetchOut)
{
    //direct memory copy
    memcpy(fetchOut.data(),
           &((channelCache[modbusAddress.getChannel()])[modbusAddress.getRegisterAddress()]),
            utilities::sizeOf(fetchOut));
}

quint16* ModbusChannel::toCacheAddress(const ModbusDriverAddress modbusAddress)
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
   //write , size unit should be converting
   writeData(modbusAddress,result.values().data(),result.valueCount()*2);

   emit raiseUpdateEvent(new UpdateEvent(modbusAddress,QVariant(channelCache[modbusAddress.getChannel()][modbusAddress.getRegisterAddress()])));
}

//!
//! \brief ModbusChannel::writeData
//! \param modbusAddress
//! \param source
//! Compatible both for QVariant , and QModbusDataUnit
void ModbusChannel::writeData(ModbusDriverAddress modbusAddress,const void* source,size_t count)
{
    //write into cache
    memcpy(toCacheAddress(modbusAddress),
           source,
           count);
}
