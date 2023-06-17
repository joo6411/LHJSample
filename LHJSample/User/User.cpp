#include "User.h"

void User::Init(const INT32 index)
{
	mIndex = index;
	mPakcetDataBuffer = new char[PACKET_DATA_BUFFER_SIZE];
}

void User::Clear()
{
	mRoomIndex = -1;
	mUserID = "";
	mIsConfirm = false;
	mCurDomainState = DOMAIN_STATE::NONE;

	mPakcetDataBufferWPos = 0;
	mPakcetDataBufferRPos = 0;
}

int User::SetLogin(char* userID_)
{
	mCurDomainState = DOMAIN_STATE::LOGIN;
	mUserID = userID_;

	return 0;
}

void User::EnterRoom(INT32 roomIndex_)
{
	mRoomIndex = roomIndex_;
	mCurDomainState = DOMAIN_STATE::ROOM;
}

void User::SetPacketData(const UINT32 dataSize_, char* pData_)
{
	if ((mPakcetDataBufferWPos + dataSize_) >= PACKET_DATA_BUFFER_SIZE)
	{
		auto remainDataSize = mPakcetDataBufferWPos - mPakcetDataBufferRPos;

		if (remainDataSize > 0)
		{
			CopyMemory(&mPakcetDataBuffer[0], &mPakcetDataBuffer[mPakcetDataBufferRPos], remainDataSize);
			mPakcetDataBufferWPos = remainDataSize;
		}
		else
		{
			mPakcetDataBufferWPos = 0;
		}

		mPakcetDataBufferRPos = 0;
	}

	CopyMemory(&mPakcetDataBuffer[mPakcetDataBufferWPos], pData_, dataSize_);
	mPakcetDataBufferWPos += dataSize_;
}

PacketInfo User::GetPacket()
{
	const int PACKET_SIZE_LENGTH = 2;
	const int PACKET_TYPE_LENGTH = 2;
	short packetSize = 0;

	UINT32 remainByte = mPakcetDataBufferWPos - mPakcetDataBufferRPos;

	if (remainByte < PACKET_HEADER_LENGTH)
	{
		return PacketInfo();
	}

	auto pHeader = (PACKET_HEADER*)&mPakcetDataBuffer[mPakcetDataBufferRPos];

	if (pHeader->PacketLength > remainByte)
	{
		return PacketInfo();
	}

	PacketInfo packetInfo;
	packetInfo.PacketId = pHeader->PacketId;
	packetInfo.DataSize = pHeader->PacketLength;
	packetInfo.pDataPtr = &mPakcetDataBuffer[mPakcetDataBufferRPos];

	mPakcetDataBufferRPos += pHeader->PacketLength;

	return packetInfo;
}