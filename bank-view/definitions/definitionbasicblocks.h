#ifndef BASICBLOCKSDEFINITION_H
#define BASICBLOCKSDEFINITION_H

#include <QObject>

#include <definitionsbaselayer.h>
#include <definitionauxiliarykeys.h>

#include <utilities.h>
using namespace BaseLayer;

//#pragma pack(1)
#define DATA_BLOCK_SIZE_IN_WORD_64 64
#define DATA_BLOCK_SIZE_IN_WORD_128 128

#define INVALID_INDEX -1

//!
//! \brief The CellDataBlock class
//! Pure storage
//template<int size>
class CellDataBlock
{
public:
    CellDataBlock()
    {
        memset(reserved,0,sizeof(MODBUS_U_WORD) * DATA_BLOCK_SIZE_IN_WORD_64);
    }
protected:
    MODBUS_U_WORD reserved[DATA_BLOCK_SIZE_IN_WORD_64]; //static , cannot not be re-direct
};
Q_DECLARE_METATYPE(CellDataBlock)



//!
//! \brief The AbstractDataBlock class
//! Resolve raw memory block in IEC
class AbstractDataBlock :
        public QObject
{
    Q_OBJECT
public:
    //!
    //! \brief AbstractDataBlock
    //!
    AbstractDataBlock(QObject* parent=nullptr) :
        QObject (parent)
    {
        m_anchor = reinterpret_cast<MODBUS_U_WORD*>(new CellDataBlock());
        m_allocated = true;

    }
    //!
    //! \brief AbstractDataBlock
    //! \param anchor
    //! \param parent
    //! Link anchor to resolve
    AbstractDataBlock(MODBUS_U_WORD* anchor,QObject* parent=nullptr) :
        QObject(parent)
    {
        m_anchor = anchor;
        m_allocated = false;
    }

    //!
    //! \brief operator =
    //! \param other
    //! \return
    //! Copy anchor to me
    AbstractDataBlock& operator=(const AbstractDataBlock& other)
    {
        if(this != &other)
            m_anchor = other.m_anchor;
        return *this;
    }
    AbstractDataBlock& operator=(const CellDataBlock& other)
    {
        memcpy(m_anchor,reinterpret_cast<const void*>(&other),sizeof (CellDataBlock));
        return *this;
    }
    MODBUS_U_WORD* Anchor() const { return m_anchor;}


    ~AbstractDataBlock()
    {
        if(m_allocated)
            delete reinterpret_cast<CellDataBlock*>(m_anchor);
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
        return QVariant::fromValue(m_anchor[key]);
    }

    static QMap<DEF_BASIC_DIMENSION::Keys,qreal>* Dimension;

protected:
    //!
    //! Generic write
    //! Long/Word/Bool
    template<typename T>
    inline void setData(uint key,const T& data)
    {
        memcpy(&m_anchor[key],&data,sizeof(T));
    }
    //!
    //! Genric read
    //!
    template<typename T>
    inline T getData(uint key) const
    {
        return *reinterpret_cast<const T*>(&m_anchor[key]);
    }
    inline void Bit(uint key,bool value)
    {
        quint16 mask = ADDRESS_BIT_ACCESSOR(key);
        if(value)
            m_anchor[ADDRESS_REGISTER(key)] |= mask;
        else
            m_anchor[ADDRESS_REGISTER(key)] &= ~mask;
    }
    inline bool Bit(uint key) const
    {
        return (m_anchor[ADDRESS_REGISTER(key)] & ADDRESS_BIT_ACCESSOR(key)) > 0;
    }

    //!
    //! \brief reserved
    //! anchor of memory block
    MODBUS_U_WORD* m_anchor;
protected:
    //!
    //! \brief m_allocated
    //! Whether anchor allocated
    bool m_allocated;
};
//Q_DECLARE_METATYPE(AbstractDataBlock)





#endif // BASICBLOCKSDEFINITION_H
