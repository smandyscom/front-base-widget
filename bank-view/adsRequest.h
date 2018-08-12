#pragma once

class AdsRequest
{
public:
	enum AdsAccess
	{
		READ,
		WRITE
	};
	

	AdsRequest();
	AdsRequest(AdsAccess access,ulong group,ulong offset,ulong length,void* cache) :
		_access(access),
		_offset(offset),
		_length(length),
		_group(group),
		_cache(cache)
	{}
	~AdsRequest();

	//Following the Ads API specicication
	AdsAccess Access() { return _access; }
	ulong Group() { return _group; }
	ulong Offset() { return _offset; }
	ulong Length() { return _length; }
	void* Cache() { return _cache; }

private:
	AdsAccess _access;
	ulong _group;
	ulong _offset;
	ulong _length;
	void* _cache; //where to store acquired values
};

AdsRequest::AdsRequest()
{
}

AdsRequest::~AdsRequest()
{
}