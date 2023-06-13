#include "EchoServer.h"
#include <vector>
#include <thread>

void EchoServer::OnConnect(const UINT32 clientIndex_)
{
	printf("[OnConnect] 클라이언트: Index(%d)\n", clientIndex_);
}

void EchoServer::OnClose(const UINT32 clientIndex_) 
{
	printf("[OnClose] 클라이언트: Index(%d)\n", clientIndex_);
}

void EchoServer::OnReceive(const UINT32 clientIndex_, const UINT32 size_, char* pData_)
{
	printf("[OnReceive] 클라이언트: Index(%d), dataSize(%d)\n", clientIndex_, size_);

	PacketData packet;
	packet.Set(clientIndex_, size_, pData_);

	std::lock_guard<std::mutex> guard(mLock);
	mPacketDataQueue.push_back(packet);
}

void EchoServer::Run(const UINT32 maxClient)
{
	mIsRunProcessThread = true;
	mProcessThread = std::thread([this]() { ProcessPacket(); });

	StartServer(maxClient);
}

void EchoServer::End()
{
	mIsRunProcessThread = false;

	if (mProcessThread.joinable())
	{
		mProcessThread.join();
	}

	DestroyThread();
}

void EchoServer::ProcessPacket()
{
	while (mIsRunProcessThread)
	{
		auto packetData = DequePacketData();
		if (packetData.DataSize != 0)
		{
			SendMsg(packetData.SessionIndex, packetData.DataSize, packetData.pPacketData);
		}
		else
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}
}

PacketData EchoServer::DequePacketData()
{
	PacketData packetData;

	std::lock_guard<std::mutex> guard(mLock);
	if (mPacketDataQueue.empty())
	{
		return PacketData();
	}

	packetData.Set(mPacketDataQueue.front());

	mPacketDataQueue.front().Release();
	mPacketDataQueue.pop_front();

	return packetData;
}