#ifndef BASICBLOCKSDEFINITION_H
#define BASICBLOCKSDEFINITION_H
#include <definitionsbaselayer.h>
#include <utilities.h>
using namespace BaseLayer;

#pragma pack(1)
#define DATA_BLOCK_SIZE_IN_WORD_64 64
#define DATA_BLOCK_SIZE_IN_WORD_128 128
class AbstractDataBlock
{
public:
    AbstractDataBlock()
    {
        memset(reserved,0,sizeof(MODBUS_WORD) * DATA_BLOCK_SIZE_IN_WORD_64);
    }

    virtual void Value(int key, QVariant value)
    {
        memcpy(&reserved[key],value.data(),utilities::sizeOf(value));
    }
    //!
    //! \brief Value
    //! \param index
    //! \return
    //! 64Bits
    virtual QVariant Value(int key) const
    {
        return QVariant::fromValue(reserved[key]);
    }



protected:
    MODBUS_WORD reserved[DATA_BLOCK_SIZE_IN_WORD_64];

    //!
    //! Generic write
    //! Long/Word/Bool
    template<typename T>
    void Data(int key,const T& data)
    {
        memcpy(&reserved[key],&data,sizeof(T));
    }
    //!
    //! Genric read
    //!
    template<typename T>
    T Data(int key)
    {
        return *static_cast<T*>(&reserved[key]);
    }
    void Bit(int key,bool value)
    {
        //!
        //! \brief mask
        //! TODO
        MODBUS_WORD mask = key;

        if(value)
            reserved[key&0xffff] |= mask;
        else
            reserved[key] &= ~mask;
    }
    bool Bit(int key) const
    {
        //!
        //! \brief mask
        //! TODO
        MODBUS_WORD mask = key;

        return (reserved[key] & mask) > 0;
    }
};
Q_DECLARE_METATYPE(AbstractDataBlock)




typedef MODBUS_LONG CommitDataBlockIndex ;

//!
//! \brief The CommitBlock class
//!
class CommitBlock
{
public:
    //!
    //! \brief The CommitSelection enum
    //! Data block selection
    enum CommitDataBlockSelection : MODBUS_LONG
    {
        SELECTION_AXIS = 0,
        SELECTION_CYLINDER = 1,
        SELECTION_COMMAND_BLOCK = 2,
        SELECTION_UNIT = 3,
    };
    enum CommitMode : MODBUS_LONG
    {
        MODE_EXE_COMMAND_BLOCK=0,
        MODE_EXE_CYLINDER=1,
        MODE_EXE_AXIS=8,
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
    MODBUS_WORD reserved[8];
};
Q_DECLARE_METATYPE(IoMonitorOverrideBlock)



#endif // BASICBLOCKSDEFINITION_H
