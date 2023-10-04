#include "User.h"
#include "State/UserState.h"
#include "../ServerNetwork/IOCPServer.h"
#include "UserManager.h"
#include "../Room/RoomManager.h"
#include "../ResultCode.h"
#include "State/UserStateConnected.h"
#include "State/UserStateDisconnect.h"
#include "State/UserStateInLobby.h"
#include "State/UserStateInRoom.h"
#include "State/UserStateNone.h"

void User::Init(const INT32 index, UserManager* userManager, RoomManager* roomManager)
{
	mIndex = index;
	mPacketDataBuffer = new char[PACKET_DATA_BUFFER_SIZE];
	mStates[UserState::None] = new UserStateNone(this);
	mStates[UserState::Connected] = new UserStateConnected(this);
	mStates[UserState::Disconnect] = new UserStateDisConnect(this);
	mStates[UserState::InLobby] = new UserStateInLobby(this);
	mStates[UserState::InRoom] = new UserStateInRoom(this);
	ChangeState(UserState::None);
	ResetRecvFunction();
	mUserManager = userManager;
	mRoomManager = roomManager;
}

void User::Clear()
{
	mRoomIndex = -1;
	mUserID = "";
	mIsConfirm = false;
	ChangeState(UserState::None);
	ResetRecvFunction();

	mPacketDataBufferWPos = 0;
	mPacketDataBufferRPos = 0;
}

int User::SetLogin(std::string& userID_)
{
	ChangeState(UserState::InLobby);
	mUserID = userID_;

	return 0;
}

RESULT_CODE User::EnterRoom(INT32 roomIndex_)
{
	RESULT_CODE result = mRoomManager->EnterUser(roomIndex_, this);
	mRoomIndex = roomIndex_;
	ChangeState(UserState::InRoom);
	return result;
}

RESULT_CODE User::LeaveRoom()
{
	RESULT_CODE result = mRoomManager->LeaveUser(mRoomIndex, this);
	ChangeState(UserState::InLobby);
	return result;
}

void User::SetPacketData(const UINT32 dataSize_, char* pData_)
{
	if ((mPacketDataBufferWPos + dataSize_) >= PACKET_DATA_BUFFER_SIZE)
	{
		auto remainDataSize = mPacketDataBufferWPos - mPacketDataBufferRPos;

		if (remainDataSize > 0)
		{
			CopyMemory(&mPacketDataBuffer[0], &mPacketDataBuffer[mPacketDataBufferRPos], remainDataSize);
			mPacketDataBufferWPos = remainDataSize;
		}
		else
		{
			mPacketDataBufferWPos = 0;
		}

		mPacketDataBufferRPos = 0;
	}

	CopyMemory(&mPacketDataBuffer[mPacketDataBufferWPos], pData_, dataSize_);
	mPacketDataBufferWPos += dataSize_;
}

PacketInfo User::GetPacket()
{
	const int PACKET_SIZE_LENGTH = 2;
	const int PACKET_TYPE_LENGTH = 2;
	short packetSize = 0;

	UINT32 remainByte = mPacketDataBufferWPos - mPacketDataBufferRPos;

	if (remainByte < PACKET_HEADER_LENGTH)
	{
		return PacketInfo();
	}

	auto pHeader = (PACKET_HEADER*)&mPacketDataBuffer[mPacketDataBufferRPos];

	if (pHeader->PacketLength > remainByte)
	{
		return PacketInfo();
	}

	PacketInfo packetInfo;
	packetInfo.PacketId = pHeader->PacketId;
	packetInfo.DataSize = pHeader->PacketLength;
	packetInfo.pDataPtr = &mPacketDataBuffer[mPacketDataBufferRPos];

	mPacketDataBufferRPos += pHeader->PacketLength;

	return packetInfo;
}

void User::ChangeState(UserState newState)
{
	switch (newState)
	{
	case UserState::None:
			return;
		break;
	}

	mStates[mState]->Exit();
	mState = newState;
	mStates[mState]->Enter();
}

void User::ResetRecvFunction()
{
	recvFuntionDictionary[(int)PACKET_ID::REQ_CREATE_ACCOUNT] = [](UINT16 packetSize, char* packet)-> void { return; };
	recvFuntionDictionary[(int)PACKET_ID::REQ_LOGIN] = [](UINT16 packetSize, char* packet)-> void { return; };
	recvFuntionDictionary[(int)PACKET_ID::REQ_LOBBY_INFO] = [](UINT16 packetSize, char* packet)-> void { return; };
	//mRecvFuntionDictionary[(int)RedisTaskID::RESPONSE_LOGIN] = &PacketManager::ProcessLoginDBResult;
	recvFuntionDictionary[(int)PACKET_ID::REQ_ROOM_ENTER] = [](UINT16 packetSize, char* packet)-> void { return; };
	recvFuntionDictionary[(int)PACKET_ID::REQ_ROOM_INFO] = [](UINT16 packetSize, char* packet)-> void { return; };
	recvFuntionDictionary[(int)PACKET_ID::REQ_ROOM_LEAVE] = [](UINT16 packetSize, char* packet)-> void { return; };
	recvFuntionDictionary[(int)PACKET_ID::REQ_ROOM_CHAT] = [](UINT16 packetSize, char* packet)-> void { return; };
}

bool User::CreateAccount(std::string& userID, std::string& password)
{
	return mUserManager->CreateAccount(userID, password);
}

RESULT_CODE User::CheckLoginable(UINT32 clientIndex_, std::string& userID, std::string& password)
{ 
	return mUserManager->CheckLoginable(clientIndex_, userID, password);
}

std::vector<int> User::GetRoomList()
{
	return mRoomManager->GetRoomList();
}

Room* User::GetRoom()
{
	return mRoomManager->GetRoomByNumber(GetCurrentRoomIndex());
}