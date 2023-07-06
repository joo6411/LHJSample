#include "ChatServer.h"

#include <vector>
#include <thread>
#include <iostream>

void ChatServer::OnConnect(const UINT32 clientIndex_)
{
	std::cout<< "[OnConnect] Ŭ���̾�Ʈ: Index(" << clientIndex_ << ")" << std::endl;

	PacketInfo packet{ clientIndex_, (UINT16)PACKET_ID::SYS_USER_CONNECT, 0 };
	mPacketManager->PushSystemPacket(packet);
}

void ChatServer::OnClose(const UINT32 clientIndex_) 
{
	std::cout << "[OnClose] Ŭ���̾�Ʈ: Index(" << clientIndex_ << ")" << std::endl;

	PacketInfo packet{ clientIndex_, (UINT16)PACKET_ID::SYS_USER_DISCONNECT, 0 };
	mPacketManager->PushSystemPacket(packet);
}

void ChatServer::OnReceive(const UINT32 clientIndex_, const UINT32 size_, char* pData_)
{
	std::cout << "[OnReceive] Ŭ���̾�Ʈ: Index(" << clientIndex_ << ", dataSize(" << size_ << std::endl;

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