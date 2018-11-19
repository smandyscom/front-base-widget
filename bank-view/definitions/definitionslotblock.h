#ifndef DEFINITIONSLOTBLOCK_H
#define DEFINITIONSLOTBLOCK_H

#include <definitionbasicblocks.h>

//!
//! \brief The CellDataBlock class
//! Pure storage
//template<int size>
class DataBlock128
{
public:
	DataBlock128()
	{
		memset(reserved, 0, sizeof(MODBUS_U_WORD) * DATA_BLOCK_SIZE_IN_WORD_128);
	}
protected:
	MODBUS_U_WORD reserved[DATA_BLOCK_SIZE_IN_WORD_128]; //static 
};
Q_DECLARE_METATYPE(DataBlock128)

//!
//! \brief The SlotDataBlock class
//! 128Words
class SlotDataBlock :
        public AbstractDataBlock
{
    Q_OBJECT
public:
	enum Offset
	{
		//! PLC->DB
		WORD_OUT = 0x0000,
		//! PLC<-DB
		WORD_IN = 0x0008,
		//! Mutual
		MATERIAL_ID = 0x0010, //move to first 8 words
		BLOCK_DATA = 0x0014, //64words occupied
	};
	Q_ENUM(Offset)
	enum Bits
	{
		BIT1_ACT = 0x10000 + WORD_OUT,
		BIT2_VALID = 0x20000 + WORD_OUT,

		BIT0_DB_ENGAGED = 0x00000+WORD_IN,
		BIT1_DONE = 0x10000+WORD_IN,
	};
	Q_ENUM(Bits)

	SlotDataBlock(QObject* parent = nullptr) 
	{
		//mandotory for AbstractSqlTableAdpater
		m_anchor = reinterpret_cast<MODBUS_U_WORD*>(new DataBlock128());
		m_allocated = true;
	}
	SlotDataBlock(MODBUS_U_WORD* anchor,QObject* parent=nullptr) :
        AbstractDataBlock(anchor,parent)
    {
		//anchor should be the base-address of 128Words consequtial memory
    }

	//!
	QVariant Value(uint key) const Q_DECL_OVERRIDE
	{
		switch (key)
		{
		case MATERIAL_ID:
			return getData<MODBUS_U_LONG>(key);
			break;
		case BLOCK_DATA:
        {
            CellDataBlock* temp = reinterpret_cast<CellDataBlock*>(&m_anchor[key]);
			return QVariant::fromValue(*temp);
			break;
        }
		default:
			return Bit(key);
			break;
		}
	}
	void Value(uint key, QVariant value) Q_DECL_OVERRIDE
	{
		switch (key)
		{
		case MATERIAL_ID:
			setData(key, value.value<MODBUS_U_LONG>());
			break;
		case BLOCK_DATA:
			setData(key, value.value<CellDataBlock>());
			break;
		default:
			Bit(key, value.toBool());
			break;
		}
	}

};
//Q_DECLARE_METATYPE(SlotDataBlock)

namespace SlotBlock {
    Q_NAMESPACE

    enum DataBaseHeaders
    {
    ID=INVALID_INDEX-1,
    //!
        DATA_0=0,
        DATA_1=1,
        DATA_2=2,
        DATA_3=3,
        DATA_4=4,
        DATA_5=5,
        DATA_6=6,
        DATA_7=7,
        DATA_8=8,
        DATA_9=9,
        DATA_10=10,
        DATA_11=11,
        DATA_12=12,
        DATA_13=13,
        DATA_14=14,
        DATA_15=15,
        DATA_16=16,
        DATA_17=17,
        DATA_18=18,
        DATA_19=19,
        DATA_20=20,
        DATA_21=21,
        DATA_22=22,
        DATA_23=23,
        DATA_24=24,
        DATA_25=25,
        DATA_26=26,
        DATA_27=27,
        DATA_28=28,
        DATA_29=29,
        DATA_30=30,
        DATA_31=31,
        DATA_32=32,
        DATA_33=33,
        DATA_34=34,
        DATA_35=35,
        DATA_36=36,
        DATA_37=37,
        DATA_38=38,
        DATA_39=39,
        DATA_40=40,
        DATA_41=41,
        DATA_42=42,
        DATA_43=43,
        DATA_44=44,
        DATA_45=45,
        DATA_46=46,
        DATA_47=47,
        DATA_48=48,
        DATA_49=49,
        DATA_50=50,
        DATA_51=51,
        DATA_52=52,
        DATA_53=53,
        DATA_54=54,
        DATA_55=55,
        DATA_56=56,
        DATA_57=57,
        DATA_58=58,
        DATA_59=59,
        DATA_60=60,
        DATA_61=61,
        DATA_62=62,
        DATA_63=63,
    };
Q_ENUM_NS(DataBaseHeaders)


}


#endif // DEFINITIONSLOTBLOCK_H
