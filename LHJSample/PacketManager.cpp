#include <utility>
#include <cstring>
#include <iostream>

#include "PacketManager.h"
#include "RedisManager.h"
#include "Config/ConfigLoader.h"
#include "User/UserManager.h"

PacketManager::~PacketManager()
{
	
}

void PacketManager::Init(const UINT32 maxClient_, UserManager* userManager)
{
	mRecvFuntionDictionary = std::unordered_map<int, PROCESS_RECV_PACKET_FUNCTION>();
	mRecvFuntionDictionary[(int)PACKET_ID::SYS_USER_CONNECT] = &PacketManager::ProcessUserConnect;
	mRecvFuntionDictionary[(int)PACKET_ID::SYS_USER_DISCONNECT] = &PacketManager::ProcessUserDisConnect;
	mRecvFuntionDictionary[(int)PACKET_ID::REQ_CREATE_ACCOUNT] = &PacketManager::ProcessCreateAccount;
	mRecvFuntionDictionary[(int)PACKET_ID::REQ_LOGIN] = &PacketManager::ProcessLogin;
	mRecvFuntionDictionary[(int)PACKET_ID::REQ_LOBBY_INFO] = &PacketManager::ProcessLobbyInfo;
	//mRecvFuntionDictionary[(int)RedisTaskID::RESPONSE_LOGIN] = &PacketManager::ProcessLoginDBResult;
	mRecvFuntionDictionary[(int)PACKET_ID::REQ_ROOM_ENTER] = &PacketManager::ProcessEnterRoom;
	mRecvFuntionDictionary[(int)PACKET_ID::REQ_ROOM_INFO] = &PacketManager::ProcessRoomInfo;
	mRecvFuntionDictionary[(int)PACKET_ID::REQ_ROOM_LEAVE] = &PacketManager::ProcessLeaveRoom;
	mRecvFuntionDictionary[(int)PACKET_ID::REQ_ROOM_CHAT] = &PacketManager::ProcessRoomChatMessage;

	/*
	mRedisMgr = new RedisManager;// std::make_unique<RedisManager>();
	mRedisMgr->Init();
	*/
	mUserManager = userManager;
}

bool PacketManager::Run()
{
	/*
	if (mRedisMgr->Run(gConfig.GetRedisServerInfo().IP, gConfig.GetRedisServerInfo().Port, 1) == false)
	{
		return false;
	}
	*/

	mIsRunProcessThread = true;
	mProcessThread = std::thread([this]() { ProcessPacket(); });

	return true;
}

void PacketManager::End()
{
	mRedisMgr->End();

	mIsRunProcessThread = false;

	if (mProcessThread.joinable())
	{
		mProcessThread.join();
	}
}

void PacketManager::ReceivePacketData(const UINT32 clientIndex_, const UINT32 size_, char* pData_)
{
	auto pUser = mUserManager->GetUserByConnIdx(clientIndex_);
	pUser->SetPacketData(size_, pData_);

	EnqueuePacketData(clientIndex_);
}

void PacketManager::ProcessPacket()
{
	while (mIsRunProcessThread)
	{
		bool isIdle = true;
		auto packetData = DequePacketData();
		if (packetData.PacketId > (UINT16)PACKET_ID::SYS_END)
		{
			isIdle = false;
			ProcessRecvPacket(packetData.ClientIndex, packetData.PacketId, packetData.DataSize, packetData.pDataPtr);
		}

		packetData = DequeSystemPacketData();
		if ( packetData.PacketId != 0)
		{
			isIdle = false;
			ProcessRecvPacket(packetData.ClientIndex, packetData.PacketId, packetData.DataSize, packetData.pDataPtr);
		}

		/*
		auto task = mRedisMgr->TakeResponseTask();
		if (task.TaskID != RedisTaskID::INVALID)
		{
			isIdle = false;
			ProcessRecvPacket(task.UserIndex, (UINT16)task.TaskID, task.DataSize, task.pData);
			task.Release();
		}

		if (isIdle)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
		*/
	}
}

void PacketManager::EnqueuePacketData(const UINT32 clientIndex_)
{
	std::lock_guard<std::mutex> guard(mLock);
	mInComingPacketUserIndex.push_back(clientIndex_);
}

PacketInfo PacketManager::DequePacketData()
{
	UINT32 userIndex = 0;

	{
		std::lock_guard<std::mutex> guard(mLock);
		if (mInComingPacketUserIndex.empty())
		{
			return PacketInfo();
		}

		userIndex = mInComingPacketUserIndex.front();
		mInComingPacketUserIndex.pop_front();
	}

	auto pUser = mUserManager->GetUserByConnIdx(userIndex);
	auto packetData = pUser->GetPacket();
	packetData.ClientIndex = userIndex;
	return packetData;
}

void PacketManager::PushSystemPacket(PacketInfo packet_)
{
	std::lock_guard<std::mutex> guard(mLock);
	mSystemPacketQueue.push_back(packet_);
}

PacketInfo PacketManager::DequeSystemPacketData()
{
	std::lock_guard<std::mutex> guard(mLock);
	if (mSystemPacketQueue.empty())
	{
		return PacketInfo();
	}

	auto packetData = mSystemPacketQueue.front();
	mSystemPacketQueue.pop_front();

	return packetData;
}

void PacketManager::ProcessRecvPacket(const UINT32 clientIndex_, const UINT16 packetId_, const UINT16 packetSize_, char* pPacket_)
{
	auto iter = mRecvFuntionDictionary.find(packetId_);
	if (iter != mRecvFuntionDictionary.end())
	{
		(this->*(iter->second))(clientIndex_, packetSize_, pPacket_);
	}
}

void PacketManager::ProcessUserConnect(UINT32 clientIndex_, UINT16 packetSize_, char* pPacket_)
{
	std::cout << "[ProcessUserConnect] clientIndex: " << clientIndex_ << std::endl;
	auto pUser = mUserManager->GetUserByConnIdx(clientIndex_);
	pUser->Clear();
	pUser->ChangeState(UserState::Connected);
}

void PacketManager::ProcessUserDisConnect(UINT32 clientIndex_, UINT16 packetSize_, char* pPacket_)
{
	std::cout << "[ProcessUserDisConnect] clientIndex: " << clientIndex_ << std::endl;
	ClearConnectionInfo(clientIndex_);
}

void PacketManager::ProcessCreateAccount(UINT32 clientIndex_, UINT16 packetSize_, char* pPacket_)
{
	auto user = mUserManager->GetUserByConnIdx(clientIndex_);
	user->recvFuntionDictionary[(int)PACKET_ID::REQ_CREATE_ACCOUNT](packetSize_, pPacket_);
}

void PacketManager::ProcessLogin(UINT32 clientIndex_, UINT16 packetSize_, char* pPacket_)
{
	std::cout << "[ProcessLogin] clientIndex: " << clientIndex_ << std::endl;
	auto user = mUserManager->GetUserByConnIdx(clientIndex_);
	user->recvFuntionDictionary[(int)PACKET_ID::REQ_LOGIN](packetSize_, pPacket_);
}

void PacketManager::ProcessLobbyInfo(UINT32 clientIndex_, UINT16 packetSize_, char* pPacket_)
{
	auto user = mUserManager->GetUserByConnIdx(clientIndex_);
	user->recvFuntionDictionary[(int)PACKET_ID::REQ_LOBBY_INFO](packetSize_, pPacket_);
}

void PacketManager::ProcessEnterRoom(UINT32 clientIndex_, UINT16 packetSize_, char* pPacket_)
{
	auto user = mUserManager->GetUserByConnIdx(clientIndex_);
	user->recvFuntionDictionary[(int)PACKET_ID::REQ_ROOM_ENTER](packetSize_, pPacket_);
}

void PacketManager::ProcessRoomInfo(UINT32 clientIndex_, UINT16 packetSize_, char* pPacket_)
{
	auto user = mUserManager->GetUserByConnIdx(clientIndex_);
	user->recvFuntionDictionary[(int)PACKET_ID::REQ_ROOM_INFO](packetSize_, pPacket_);
}

void PacketManager::ProcessLeaveRoom(UINT32 clientIndex_, UINT16 packetSize_, char* pPacket_)
{
	auto user = mUserManager->GetUserByConnIdx(clientIndex_);
	user->recvFuntionDictionary[(int)PACKET_ID::REQ_ROOM_LEAVE](packetSize_, pPacket_);
}


void PacketManager::ProcessRoomChatMessage(UINT32 clientIndex_, UINT16 packetSize_, char* pPacket_)
{
	auto user = mUserManager->GetUserByConnIdx(clientIndex_);
	user->recvFuntionDictionary[(int)PACKET_ID::REQ_ROOM_CHAT](packetSize_, pPacket_);
}

void PacketManager::ClearConnectionInfo(INT32 clientIndex_)
{
	auto pReqUser = mUserManager->GetUserByConnIdx(clientIndex_);

	if (pReqUser->GetState() != UserState::None)
	{
		mUserManager->DeleteUserInfo(pReqUser);
	}
}