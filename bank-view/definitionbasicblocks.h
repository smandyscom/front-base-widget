#ifndef BASICBLOCKSDEFINITION_H
#define BASICBLOCKSDEFINITION_H

#include <definitionsbaselayer.h>
#include <definitionauxiliarykeys.h>

#include <utilities.h>
using namespace BaseLayer;

#pragma pack(1)
#define DATA_BLOCK_SIZE_IN_WORD_64 64
#define DATA_BLOCK_SIZE_IN_WORD_128 128

#define BASE_INDEX 0

//!
//! \brief The CellDataBlock class
//! Pure storage
//template<int size>
class CellDataBlock
{
public:
    CellDataBlock() {}
protected:
    MODBUS_U_WORD reserved[DATA_BLOCK_SIZE_IN_WORD_64];
};
Q_DECLARE_METATYPE(CellDataBlock)

//template<int size>
class AbstractDataBlock : public CellDataBlock
{
public:
    AbstractDataBlock()
    {
        memset(reserved,0,sizeof(MODBUS_U_WORD) * DATA_BLOCK_SIZE_IN_WORD_64);
    }

    virtual void Value(uint key, QVariant value)
    {
        setData(key,value.value<MODBUS_U_WORD>());
    }
    //!
    //! \brief Value
    //! \param index
    //! \return
    //! 64Bits
    virtual QVariant Value(uint key) const
    {
        return QVariant::fromValue(reserved[key]);
    }

    static QMap<DEF_BASIC_DIMENSION::Keys,qreal> *Dimension;

protected:
    //MODBUS_U_WORD reserved[DATA_BLOCK_SIZE_IN_WORD_64];

    //!
    //! Generic write
    //! Long/Word/Bool
    template<typename T>
    inline void setData(uint key,const T& data)
    {
        memcpy(&reserved[key],&data,sizeof(T));
    }
    //!
    //! Genric read
    //!
    template<typename T>
    inline T getData(uint key) const
    {
        return *reinterpret_cast<const T*>(&reserved[key]);
    }
    inline void Bit(uint key,bool value)
    {
        //!
        //! \brief mask
        ModbusDriverAddress __address(key);

        if(value)
            reserved[__address.getRegisterAddress()] |= __address.toBitwiseMask();
        else
            reserved[__address.getRegisterAddress()] &= ~__address.toBitwiseMask();
    }
    inline bool Bit(uint key) const
    {
        //!
        //! \brief mask
        ModbusDriverAddress __address(key);

        return (reserved[__address.getRegisterAddress()] & __address.toBitwiseMask()) > 0;
    }
};
Q_DECLARE_METATYPE(AbstractDataBlock)




typedef MODBUS_U_LONG CommitDataBlockIndex ;

//!
//! \brief The CommitBlock class
//!
class CommitBlock
{
public:
    //!
    //! \brief The CommitSelection enum
    //! Data block selection
    enum CommitDataBlockSelection : MODBUS_U_LONG
    {
        SELECTION_AXIS = 0,
        SELECTION_CYLINDER = 1,
        SELECTION_SIGNAL,

        SELECTION_COMMAND_BLOCK = 2,

        SELECTION_UNIT = 3,
    };
    enum CommitMode : MODBUS_U_LONG
    {
        MODE_EXE_COMMAND_BLOCK=0,
        MODE_EXE_CYLINDER=1,
        MODE_EXE_AXIS=8,

        MODE_EXE_UNIT,

        MODE_DOWNLOAD_DATA_BLOCK = 15, //PLC<-HMI
        MODE_UPLOAD_DATA_BLOCK = 16,   //PLC->HMI
    };
    CommitBlock()
    {
        __mode = MODE_EXE_COMMAND_BLOCK;
        __selection = SELECTION_COMMAND_BLOCK;
        __index = 0;
    }

    void Mode(CommitMode value) {__mode = value;}
    CommitMode Mode() const {return __mode;}
    void Selection(CommitDataBlockSelection value) {__selection = value;}
    CommitDataBlockSelection Selection() const {return __selection;}
    void Index(CommitDataBlockIndex value) {__index = value;}
    CommitDataBlockIndex Index() const {return __index;}
protected:
    CommitMode __mode;
    CommitDataBlockSelection __selection;
    CommitDataBlockIndex __index;
};
Q_DECLARE_METATYPE(CommitBlock)



class IoMonitorOverrideBlock
{
public:
protected:
    MODBUS_U_WORD reserved[8];
};
Q_DECLARE_METATYPE(IoMonitorOverrideBlock)



#endif // BASICBLOCKSDEFINITION_H
