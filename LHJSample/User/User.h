#pragma once
#include <string>

#include "../Packet.h"

class User
{
	const UINT32 PACKET_DATA_BUFFER_SIZE = 8096;

public:
	enum class DOMAIN_STATE
	{
		NONE = 0,
		LOGIN = 1,
		ROOM = 2
	};

	User() = default;
	~User() = default;

	void Init(const INT32 index);
	void Clear();
	int SetLogin(char* userID_);
	void EnterRoom(INT32 roomIndex_);
	void SetDomainState(DOMAIN_STATE value_) { mCurDomainState = value_; }
	INT32 GetCurrentRoom() { return mRoomIndex; }
	INT32 GetNetConnIdx() { return mIndex; }

	std::string GetUserId() const { return  mUserID; }

	DOMAIN_STATE GetDomainState() { return mCurDomainState; }
	void SetPacketData(const UINT32 dataSize_, char* pData_);
	PacketInfo GetPacket();

private:
	INT32 mIndex = -1;

	INT32 mRoomIndex = -1;

	std::string mUserID;
	bool mIsConfirm = false;
	std::string mAuthToken;

	DOMAIN_STATE mCurDomainState = DOMAIN_STATE::NONE;

	UINT32 mPakcetDataBufferWPos = 0;
	UINT32 mPakcetDataBufferRPos = 0;
	char* mPakcetDataBuffer = nullptr;
};