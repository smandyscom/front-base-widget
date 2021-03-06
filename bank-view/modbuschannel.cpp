#include "modbuschannel.h"
#include <utilities.h>
#include <QCoreApplication>
#include <QDebug>
ModbusChannel::ModbusChannel(QList<ModbusSerializedClient *> channelList, QObject *parent) :
    QObject(parent)
{
    //! initialize whole inner cache anyway
    for(int i=0;i<CHANNEL_MAX_NUM;i++)
    {
        quint16* __cache = new quint16[USHRT_MAX];
        memset(__cache,0,USHRT_MAX); // cache initialization
        channelCache.append(__cache);
    }
    //! link
    for(int i=0;i<channelList.count();i++){
        channelGateWays.append(const_cast<ModbusSerializedClient*>(channelList[i]));
        connect(channelList[i],&ModbusSerializedClient::readRequestDone,this,&ModbusChannel::onReadRequestProcessed);
    }
    preparedReadRequest.setRegisterType(QModbusDataUnit::HoldingRegisters);
    preparedWriteRequest.setRegisterType(QModbusDataUnit::HoldingRegisters);
}

void ModbusChannel::beginAccess(ModbusDriverAddress address, const QVariant dataForm)
{
    // From map query type of QVariant
    // Get the object size , so that you can make right request
    size_t sizeInWord = (utilities::sizeOf(dataForm))/2; //size of in byte

    if(sizeInWord == 0)
        sizeInWord = 1; //prevent zero count

    //! Support DiscreteInputs/HoldingRegister only
    switch (address.getRegisterType()) {
    case QModbusDataUnit::DiscreteInputs:
    case QModbusDataUnit::InputRegisters:
        preparedReadRequest.setRegisterType(address.getRegisterType());
        break;
    default:
        preparedReadRequest.setRegisterType(QModbusDataUnit::HoldingRegisters);
        break;
    }
    preparedReadRequest.setStartAddress(address.getRegisterAddress());
    preparedReadRequest.setValueCount(sizeInWord);

    //once no specific channel , do no action
    if(address.getChannel() >= channelGateWays.size())
        return;
    channelGateWays[address.getChannel()]->pushRequest(new ModbusSerializedClient::ModbusRequest(preparedReadRequest,ModbusSerializedClient::READ));
}

//!
//! \brief ModbusChannel::commit
//! \param address
//! \param value
//! handling bit?
void ModbusChannel::__commit(ModbusDriverAddress address, const QVariant value)
{
    QVariant writeInData = value;


    //!
    //! Handling bit access request
    switch(writeInData.type()){
    case QVariant::Bool:
    {
        //make sure readout a word
        MODBUS_S_WORD temp=0;
        writeInData.setValue(temp);

        __update(address,writeInData);
        // handling signal bit
        if(value.value<bool>())
            //set
            writeInData.setValue(static_cast<MODBUS_U_WORD>(writeInData.value<MODBUS_U_WORD>() | address.toBitwiseMask()));
        else
            //unset
            writeInData.setValue(static_cast<MODBUS_U_WORD>(writeInData.value<MODBUS_U_WORD>() & (~address.toBitwiseMask())));
        break;
    }
    default:
        break;
    }

    //
    // From map query type of QVariant
    // Get the object size , so that you can make right request
    size_t sizeInWord = (utilities::sizeOf(writeInData))/2; //size of in byte

    if(sizeInWord == 0)
        sizeInWord = 1; //prevent zero count

    //! Translate to MODBUSUNIT
    preparedWriteRequest.setStartAddress(address.getRegisterAddress());
    preparedWriteRequest.setValueCount(sizeInWord);
    QVector<quint16> temp;
    for(size_t i=0;i<sizeInWord;i++)
        temp.append(reinterpret_cast<const quint16*>(writeInData.data())[i]);

    preparedWriteRequest.setValues(temp);

    //once no specific channel , do no action
    if(address.getChannel() >= channelGateWays.size() )
        return;

    //! Write into cache
    writeData(address,writeInData.data(),utilities::sizeOf(writeInData));
    channelGateWays[address.getChannel()]->pushRequest(new ModbusSerializedClient::ModbusRequest(preparedWriteRequest,ModbusSerializedClient::WRITE));
}

void ModbusChannel::__update(const ModbusDriverAddress modbusAddress, QVariant &fetchOut) const
{
    //!
    //! For bool type need specical handling
    switch(fetchOut.type()){
    case QVariant::Bool:
    {
        quint16 temp = (channelCache[modbusAddress.getChannel()])[modbusAddress.getRegisterAddress()];
        if(temp & modbusAddress.toBitwiseMask())
            fetchOut.setValue(true);
        else
            fetchOut.setValue(false);
        break;
    }
    default:
        //direct memory copy
        memcpy(fetchOut.data(),
               &((channelCache[modbusAddress.getChannel()])[modbusAddress.getRegisterAddress()]),
                utilities::sizeOf(fetchOut));
        break;
    }
}

quint16* ModbusChannel::toCacheAddress(const ModbusDriverAddress modbusAddress)
{
    return &(channelCache[modbusAddress.getChannel()])[modbusAddress.getRegisterAddress()];
}

//!
//! \brief ModbusChannel::onReplyUpdated
//! \param result
//!
void ModbusChannel::onReadRequestProcessed(QModbusDataUnit result)
{
   //identify which channel?
   int channelIndex = channelGateWays.indexOf(qobject_cast<ModbusSerializedClient*>(sender()));
   //recover the address
   ModbusDriverAddress modbusAddress;
   modbusAddress.setChannel(channelIndex);
   modbusAddress.setRegisterAddress(result.startAddress());

   switch (result.registerType()) {
   case QModbusDataUnit::DiscreteInputs:
   case QModbusDataUnit::InputRegisters:
       modbusAddress.setRegisterType(result.registerType());
      break;
   default:
       //!Do nothing
       break;
   }

   //write , size unit should be converting
   writeData(modbusAddress,result.values().data(),result.valueCount()*2);

   __cachedReplyAddress = modbusAddress;
   __cachedReplyValue = QVariant(channelCache[modbusAddress.getChannel()][modbusAddress.getRegisterAddress()]);
   //!
   //! Value contains single word only
   //! Bit mask would vanished here
   emit readReply();
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

ModbusChannel* ModbusChannel::Instance()
{
    if(__instance==nullptr)
        __instance = new ModbusChannel(__channels);

    return __instance;
}
void ModbusChannel::Channels(QList<ModbusSerializedClient *> value)
{
    ModbusChannel::__channels = value;
}
QList<ModbusSerializedClient*> ModbusChannel::Channels()
{
    return ModbusChannel::__channels;
}

ModbusChannel* ModbusChannel::__instance = nullptr;
QList<ModbusSerializedClient*> ModbusChannel::__channels;
