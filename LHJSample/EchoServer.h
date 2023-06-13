#pragma once

#include "ServerNetwork/IOCPServer.h"
#include "Packet.h"
#include <deque>
#include <mutex>

class EchoServer : public IOCPServer
{
public:
	EchoServer() = default;
	virtual ~EchoServer() = default;

	virtual void OnConnect(const UINT32 clientIndex_) override;
	virtual void OnClose(const UINT32 clientIndex_) override;
	virtual void OnReceive(const UINT32 clientIndex_, const UINT32 size_, char* pData_) override;

	void Run(const UINT32 maxClient);
	void End();

private:
	void ProcessPacket();
	PacketData DequePacketData();

	bool mIsRunProcessThread = false;
	std::thread mProcessThread;
	std::mutex mLock;
	std::deque<PacketData> mPacketDataQueue;
};