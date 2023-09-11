#include "ChatServer.h"
#include "User/UserManager.h"
#include "Room/RoomManager.h"
#include "DB/AccountDB.h"
#include "PacketManager.h"
#include <vector>
#include <thread>
#include <iostream>

ChatServer::~ChatServer()
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

	if (mAccountDB)
	{
		delete mAccountDB;
		mAccountDB = nullptr;
	}

	if (mPacketManager)
	{
		delete mPacketManager;
		mPacketManager = nullptr;
	}
}

void ChatServer::Init(const UINT32 maxClient)
{
	UINT32 startRoomNumber = 0;
	UINT32 maxRoomCount = 10;
	UINT32 maxRoomUserCount = 4;

	mRoomManager = new RoomManager;
	mRoomManager->Init(startRoomNumber, maxRoomCount, maxRoomUserCount);

	mAccountDB = new AccountDB();
	mAccountDB->Init();

	mUserManager = new UserManager;
	mUserManager->Init(maxClient, mAccountDB, mRoomManager);

	mPacketManager = new PacketManager();
	mPacketManager->Init(maxClient, mUserManager);
}

void ChatServer::OnConnect(const UINT32 clientIndex_)
{
	std::cout<< "[OnConnect] 클라이언트: Index(" << clientIndex_ << ")" << std::endl;

	PacketInfo packet{ clientIndex_, (UINT16)PACKET_ID::SYS_USER_CONNECT, 0 };
	mPacketManager->PushSystemPacket(packet);
}

void ChatServer::OnClose(const UINT32 clientIndex_) 
{
	std::cout << "[OnClose] 클라이언트: Index(" << clientIndex_ << ")" << std::endl;

	PacketInfo packet{ clientIndex_, (UINT16)PACKET_ID::SYS_USER_DISCONNECT, 0 };
	mPacketManager->PushSystemPacket(packet);
}

void ChatServer::OnReceive(const UINT32 clientIndex_, const UINT32 size_, char* pData_)
{
	std::cout << "[OnReceive] 클라이언트: Index(" << clientIndex_ << "), dataSize(" << size_ <<  ")" << std::endl;

	mPacketManager->ReceivePacketData(clientIndex_, size_, pData_);
}

bool ChatServer::Run(const UINT32 maxClient)
{
	auto sendPacketFunc = [&](UINT32 clientIndex_, UINT16 packetSize, char* pSendPacket)
	{
		SendMsg(clientIndex_, packetSize, pSendPacket);
	};

	mPacketManager->SendPacketFunc = sendPacketFunc;
	mRoomManager->SetSendPacketFunc(sendPacketFunc);
	//mRoomManager->SendPacketFunc = mPacketManager->SendPacketFunc;
	mUserManager->SetSendPacketFunc(sendPacketFunc);
	
	if (!mPacketManager->Run())
	{
		return false;
	}

	StartServer(maxClient);
	return true;
}

void ChatServer::End()
{
	mPacketManager->End();

	DestroyThread();
}
