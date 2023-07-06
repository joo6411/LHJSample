#include <utility>
#include <cstring>
#include <iostream>

#include "User/UserManager.h"
#include "PacketManager.h"
#include "RedisManager.h"
#include "Room/RoomManager.h"
#include "Config/ConfigLoader.h"
#include "DB/AccountDB.h"

PacketManager::~PacketManager()
{
	if (mUserManager)
	{
		delete mUserManager;
		mUserManager = nullptr;
	}

	if (mRoomManager)
	{
		delete mRoomManager;
		mUserManager = nullptr;
	}

	if (mRedisMgr)
	{
		delete mRedisMgr;
		mRedisMgr = nullptr;
	}

	if (mAccountDB)
	{
		delete mAccountDB;
		mAccountDB = nullptr;
	}
}

void PacketManager::Init(const UINT32 maxClient_)
{
	mRecvFuntionDictionary = std::unordered_map<int, PROCESS_RECV_PACKET_FUNCTION>();

	mRecvFuntionDictionary[(int)PACKET_ID::SYS_USER_CONNECT] = &PacketManager::ProcessUserConnect;
	mRecvFuntionDictionary[(int)PACKET_ID::SYS_USER_DISCONNECT] = &PacketManager::ProcessUserDisConnect;
	
	mRecvFuntionDictionary[(int)PACKET_ID::REQ_CREATE_ACCOUNT] = &PacketManager::ProcessCreateAccount;
	mRecvFuntionDictionary[(int)PACKET_ID::REQ_LOGIN] = &PacketManager::ProcessLogin;
	//mRecvFuntionDictionary[(int)RedisTaskID::RESPONSE_LOGIN] = &PacketManager::ProcessLoginDBResult;

	mRecvFuntionDictionary[(int)PACKET_ID::REQ_ROOM_ENTER] = &PacketManager::ProcessEnterRoom;

	mRecvFuntionDictionary[(int)PACKET_ID::REQ_ROOM_INFO] = &PacketManager::ProcessRoomInfo;

	mRecvFuntionDictionary[(int)PACKET_ID::REQ_ROOM_LEAVE] = &PacketManager::ProcessLeaveRoom;
	mRecvFuntionDictionary[(int)PACKET_ID::REQ_ROOM_CHAT] = &PacketManager::ProcessRoomChatMessage;

	CreateComponent(maxClient_);
}

void PacketManager::CreateComponent(const UINT32 maxClient_)
{
	mUserManager = new UserManager;
	mUserManager->Init(maxClient_);

	mRedisMgr = new RedisManager;// std::make_unique<RedisManager>();
	mRedisMgr->Init();

	UINT32 startRoomNumber = 0;
	UINT32 maxRoomCount = 10;
	UINT32 maxRoomUserCount = 4;
	mRoomManager = new RoomManager;
	mRoomManager->SendPacketFunc = SendPacketFunc;
	mRoomManager->Init(startRoomNumber, maxRoomCount, maxRoomUserCount);

	mAccountDB = new AccountDB();
	mAccountDB->Init();
}

bool PacketManager::Run()
{
	if (mRedisMgr->Run(gConfig.GetRedisServerInfo().IP, gConfig.GetRedisServerInfo().Port, 1) == false)
	{
		return false;
	}

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
}

void PacketManager::ProcessUserDisConnect(UINT32 clientIndex_, UINT16 packetSize_, char* pPacket_)
{
	std::cout << "[ProcessUserDisConnect] clientIndex: " << clientIndex_ << std::endl;
	ClearConnectionInfo(clientIndex_);
}

void PacketManager::ProcessCreateAccount(UINT32 clientIndex_, UINT16 packetSize_, char* pPacket_)
{
	if (REQ_CREATE_ACCOUNT_PACKET_SIZE != packetSize_)
	{
		return;
	}

	auto packet = reinterpret_cast<REQ_CREATE_ACCOUNT_PACKET*>(pPacket_);
	RESULT_CODE err = RESULT_CODE::NONE;
	if (!mAccountDB->CreateAccount(packet->UserID, packet->UserPW))
	{
		err = RESULT_CODE::CREATE_ACCOUNT_FAIL;
	}

	ACK_CREATE_ACCOUNT_PACKET createAccountPacket;
	createAccountPacket.PacketId = (UINT16)PACKET_ID::ACK_CREATE_ACCOUNT;
	createAccountPacket.Result = (UINT16)err;
	createAccountPacket.PacketLength = sizeof(ACK_CREATE_ACCOUNT_PACKET);
	
	SendPacketFunc(clientIndex_, sizeof(ACK_CREATE_ACCOUNT_PACKET), (char*)&createAccountPacket);
}

void PacketManager::ProcessLogin(UINT32 clientIndex_, UINT16 packetSize_, char* pPacket_)
{
	if (REQ_LOGIN_PACKET_SIZE != packetSize_)
	{
		return;
	}

	auto packet = reinterpret_cast<REQ_LOGIN_PACKET*>(pPacket_);
	auto pUserID = packet->UserID;
	std::cout << "requested user id = " << pUserID << std::endl;

	ACK_LOGIN_PACKET ackLoginPacket;
	ackLoginPacket.PacketId = (UINT16)PACKET_ID::ACK_LOGIN;
	ackLoginPacket.PacketLength = sizeof(ACK_LOGIN_PACKET);

	if (mUserManager->GetCurrentUserCnt() >= mUserManager->GetMaxUserCnt())
	{
		//접속자수가 최대수를 차지해서 접속불가
		ackLoginPacket.Result = (UINT16)RESULT_CODE::LOGIN_USER_USED_ALL_OBJ;
		return;
	}

	//여기에서 이미 접속된 유저인지 확인하고, 접속된 유저라면 실패한다.
	if (mUserManager->FindUserIndexByID(pUserID) == -1)
	{
		/*
		RedisLoginReq dbReq;
		CopyMemory(dbReq.UserID, pLoginReqPacket->UserID, (MAX_USER_ID_LEN + 1));
		CopyMemory(dbReq.UserPW, pLoginReqPacket->UserPW, (MAX_USER_PW_LEN + 1));

		RedisTask task;
		task.UserIndex = clientIndex_;
		task.TaskID = RedisTaskID::REQUEST_LOGIN;
		task.DataSize = sizeof(RedisLoginReq);
		task.pData = new char[task.DataSize];
		CopyMemory(task.pData, (char*)&dbReq, task.DataSize);
		mRedisMgr->PushTask(task);

		std::cout<<("Login To Redis user id = %s\n", pUserID);
		*/

		if (mAccountDB->SelectAccount(packet->UserID, packet->UserPW))
		{
			mUserManager->AddUser(packet->UserID, clientIndex_);
			ackLoginPacket.Result = (UINT16)RESULT_CODE::LOGIN_SUCCESS;
		}
		else
		{
			ackLoginPacket.Result = (UINT16)RESULT_CODE::LOGIN_USER_INVALID_PW;
		}
	}
	else
	{
		//접속중인 유저여서 실패를 반환한다.
		ackLoginPacket.Result = (UINT16)RESULT_CODE::LOGIN_USER_ALREADY;
		return;
	}

	SendPacketFunc(clientIndex_, sizeof(ACK_LOGIN_PACKET), (char*)&ackLoginPacket);
}

void PacketManager::ProcessEnterRoom(UINT32 clientIndex_, UINT16 packetSize_, char* pPacket_)
{
	auto roomEnterReqPacket = reinterpret_cast<REQ_ROOM_ENTER_PACKET*>(pPacket_);
	auto reqUser = mUserManager->GetUserByConnIdx(clientIndex_);

	if (!reqUser || reqUser == nullptr)
	{
		return;
	}

	ACK_ROOM_ENTER_PACKET roomEnterAckPacket;
	roomEnterAckPacket.PacketId = (UINT16)PACKET_ID::ACK_ROOM_ENTER;
	roomEnterAckPacket.PacketLength = sizeof(ACK_ROOM_ENTER_PACKET);
	roomEnterAckPacket.Result = mRoomManager->EnterUser(roomEnterReqPacket->RoomNumber, reqUser);

	SendPacketFunc(clientIndex_, sizeof(ACK_ROOM_ENTER_PACKET), (char*)&roomEnterAckPacket);
	std::cout<<("ACK_ROOM_ENTER_PACKET Sended");
}

void PacketManager::ProcessRoomInfo(UINT32 clientIndex_, UINT16 packetSize_, char* pPacket_)
{
	auto packet = reinterpret_cast<REQ_ROOM_INFO_PACKET*>(pPacket_);
	User* user = mUserManager->GetUserByConnIdx(clientIndex_);
	Room* room = mRoomManager->GetRoomByNumber(user->GetCurrentRoom());
	
	ACK_ROOM_INFO_PACKET roomInfoAckPacket;
	roomInfoAckPacket.PacketId = (UINT16)PACKET_ID::ACK_ROOM_INFO;
	roomInfoAckPacket.PacketLength = sizeof(ACK_ROOM_INFO_PACKET);
	roomInfoAckPacket.Result = (UINT16)RESULT_CODE::NONE;
	roomInfoAckPacket.Users = room->GetRoomMemberList();

	SendPacketFunc(clientIndex_, sizeof(ACK_ROOM_INFO_PACKET), (char*)&roomInfoAckPacket);
	std::cout << ("ACK_ROOM_INFO_PACKET Sended");
}


void PacketManager::ProcessLeaveRoom(UINT32 clientIndex_, UINT16 packetSize_, char* pPacket_)
{
	ACK_ROOM_LEAVE_PACKET roomLeaveResPacket;
	roomLeaveResPacket.PacketId = (UINT16)PACKET_ID::ACK_ROOM_LEAVE;
	roomLeaveResPacket.PacketLength = sizeof(ACK_ROOM_LEAVE_PACKET);

	auto reqUser = mUserManager->GetUserByConnIdx(clientIndex_);
	auto roomNum = reqUser->GetCurrentRoom();

	//TODO Room안의 UserList객체의 값 확인하기
	roomLeaveResPacket.Result = mRoomManager->LeaveUser(roomNum, reqUser);
	SendPacketFunc(clientIndex_, sizeof(ACK_ROOM_LEAVE_PACKET), (char*)&roomLeaveResPacket);
}


void PacketManager::ProcessRoomChatMessage(UINT32 clientIndex_, UINT16 packetSize_, char* pPacket_)
{
	auto roomChatReqPacketet = reinterpret_cast<REQ_ROOM_CHAT_PACKET*>(pPacket_);

	ACK_ROOM_CHAT_PACKET roomChatAckPacket;
	roomChatAckPacket.PacketId = (UINT16)PACKET_ID::ACK_ROOM_CHAT;
	roomChatAckPacket.PacketLength = sizeof(ACK_ROOM_CHAT_PACKET);
	roomChatAckPacket.Result = (INT16)RESULT_CODE::NONE;

	auto reqUser = mUserManager->GetUserByConnIdx(clientIndex_);
	auto roomNum = reqUser->GetCurrentRoom();

	auto room = mRoomManager->GetRoomByNumber(roomNum);
	if (room == nullptr)
	{
		roomChatAckPacket.Result = (INT16)RESULT_CODE::CHAT_ROOM_INVALID_ROOM_NUMBER;
		SendPacketFunc(clientIndex_, sizeof(ACK_ROOM_CHAT_PACKET), (char*)&roomChatAckPacket);
		return;
	}

	SendPacketFunc(clientIndex_, sizeof(ACK_ROOM_CHAT_PACKET), (char*)&roomChatAckPacket);

	room->NotifyChat(clientIndex_, reqUser->GetUserId().c_str(), roomChatReqPacketet->Message);
}

void PacketManager::ClearConnectionInfo(INT32 clientIndex_)
{
	auto pReqUser = mUserManager->GetUserByConnIdx(clientIndex_);

	if (pReqUser->GetDomainState() != User::DOMAIN_STATE::NONE)
	{
		mUserManager->DeleteUserInfo(pReqUser);
	}
}