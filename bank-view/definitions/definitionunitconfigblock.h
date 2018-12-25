#pragma once

#include<definitionbasicblocks.h>

#include<abstractsqltableadpater.h>

#include<definitionauxiliarykeys.h>
using namespace DEF_BASIC_DIMENSION;

class UnitConfig :
	public AbstractDataBlock
{
	Q_OBJECT

public:
	enum OffsetConfig
	{
		OFFSET_CONFIG_UNIT_ID =0,
		OFFSET_CONFIG_CONFIG_ID=1,
		OFFSET_CONFIG_VALUE=2,
	};
	Q_ENUM(OffsetConfig)

	UnitConfig(QObject* parent = nullptr) :
		AbstractDataBlock(parent)
	{}
	UnitConfig(MODBUS_U_WORD* anchor, QObject* parent = nullptr) :
		AbstractDataBlock(anchor, parent)
	{}

	QVariant Value(uint key) const Q_DECL_OVERRIDE
	{
		switch (key)
		{
		case OFFSET_CONFIG_VALUE:
			return getData<MODBUS_U_LONG>(key);
			break;
		default:
			return AbstractDataBlock::Value(key);
			break;
		}
	}

	void Value(uint key, QVariant value) Q_DECL_OVERRIDE
	{
		switch (key)
		{
		case OFFSET_CONFIG_VALUE:
			setData(key, value.value<MODBUS_U_LONG>());
			break;
		default:
			AbstractDataBlock::Value(key, value);
			break;
		}
	}
};



namespace UnitConfigBlock {
	Q_NAMESPACE
		enum DataBaseHeaders
	{
		ID = INVALID_INDEX - 1,
		UNIT_NAME = INVALID_INDEX - 3,
		NAME = INVALID_INDEX - 4,
		UNIT_ID = INVALID_INDEX - 5,
		CONFIG_ID = INVALID_INDEX - 6,
		//!
		VALUE = UnitConfig::OFFSET_CONFIG_VALUE,
	};
	Q_ENUM_NS(DataBaseHeaders)


}//namespace