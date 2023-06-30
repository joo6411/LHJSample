#include "ChatServer.h"

#include <vector>
#include <thread>
#include <iostream>

void ChatServer::OnConnect(const UINT32 clientIndex_)
{
	std::cout<<("[OnConnect] 클라이언트: Index(%d)\n", clientIndex_);

	PacketInfo packet{ clientIndex_, (UINT16)PACKET_ID::SYS_USER_CONNECT, 0 };
	mPacketManager->PushSystemPacket(packet);
}

void ChatServer::OnClose(const UINT32 clientIndex_) 
{
	std::cout<<("[OnClose] 클라이언트: Index(%d)\n", clientIndex_);

	PacketInfo packet{ clientIndex_, (UINT16)PACKET_ID::SYS_USER_DISCONNECT, 0 };
	mPacketManager->PushSystemPacket(packet);
}

void ChatServer::OnReceive(const UINT32 clientIndex_, const UINT32 size_, char* pData_)
{
	std::cout<<("[OnReceive] 클라이언트: Index(%d), dataSize(%d)\n", clientIndex_, size_);

	mPacketManager->ReceivePacketData(clientIndex_, size_, pData_);
}

bool ChatServer::Run(const UINT32 maxClient)
{
	auto sendPacketFunc = [&](UINT32 clientIndex_, UINT16 packetSize, char* pSendPacket)
	{
		SendMsg(clientIndex_, packetSize, pSendPacket);
	};

	mPacketManager = std::make_unique<PacketManager>();
	mPacketManager->SendPacketFunc = sendPacketFunc;
	mPacketManager->Init(maxClient);
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