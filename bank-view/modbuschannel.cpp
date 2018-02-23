#include "modbuschannel.h"
#include <utilities.h>
#include <QCoreApplication>

ModbusChannel::ModbusChannel(const ModbusSerializedClient* channelList[], const int channelCounts, QObject *parent) :
    QObject(parent)
{
    for(int i=0;i<channelCounts;i++){
        channelCache.append(new quint16[USHRT_MAX]);
        channelGateWays.append(const_cast<ModbusSerializedClient*>(channelList[i]));
        //
        connect(channelList[i],&ModbusSerializedClient::requestDone,this,&ModbusChannel::onRequestProcessed);
    }
    preparedReadRequest.setRegisterType(QModbusDataUnit::HoldingRegisters);
    preparedWriteRequest.setRegisterType(QModbusDataUnit::HoldingRegisters);
}

void ModbusChannel::beginUpdate(ModbusDriverAddress address, const QVariant dataForm)
{
    // From map query type of QVariant
    // Get the object size , so that you can make right request
    size_t sizeInWord = (utilities::sizeOf(dataForm))/2; //size of in byte

    if(sizeInWord == 0)
        sizeInWord = 1; //prevent zero count

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
void ModbusChannel::commit(ModbusDriverAddress address, const QVariant value)
{
    QVariant writeInData = value;


    //!
    //! Handling bit access request
    switch(writeInData.type()){
    case QVariant::Bool:
    {
        //make sure readout a word
        quint16 temp=0;
        writeInData.setValue(temp);

        update(address,writeInData);
        // handling signal bit
        if(value.value<bool>())
            //set
            writeInData.setValue(writeInData.value<quint16>() | address.toBitwiseMask());
        else
            //unset
            writeInData.setValue(writeInData.value<quint16>() & (~address.toBitwiseMask()));
        break;
    }
    default:
        break;
    }
    //
    // From map query type of QVariant
    // Get the object size , so that you can make right request
    size_t sizeInWord = (utilities::sizeOf(writeInData))/2; //size of in byte

    preparedWriteRequest.setStartAddress(address.getRegisterAddress());
    QVector<quint16> temp;
    for(int i=0;i<sizeInWord;i++)
        temp.append(reinterpret_cast<const quint16*>(writeInData.data())[i]);

    preparedWriteRequest.setValues(temp);

    //once no specific channel , do no action
    if(address.getChannel() >= channelGateWays.size() )
        return;
    channelGateWays[address.getChannel()]->pushRequest(new ModbusSerializedClient::ModbusRequest(preparedWriteRequest,ModbusSerializedClient::WRITE));
}

void ModbusChannel::update(const ModbusDriverAddress modbusAddress, QVariant &fetchOut)
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
void ModbusChannel::onRequestProcessed(QModbusDataUnit result)
{
   //identify which channel?
   int channelIndex = channelGateWays.indexOf(qobject_cast<ModbusSerializedClient*>(sender()));
   //recover the address
   ModbusDriverAddress modbusAddress;
   modbusAddress.setChannel(channelIndex);
   modbusAddress.setRegisterAddress(result.startAddress());
   //write , size unit should be converting
   writeData(modbusAddress,result.values().data(),result.valueCount()*2);

   //!
   //! Value contains single word only
   //! Bit mask would vanished here
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
