#pragma once

#include "ServerNetwork/IOCPServer.h"
#include "Packet.h"
#include <deque>
#include <mutex>

class AccountDB;
class RoomManager;
class RedisManager;
class UserManager;
class PacketManager;

class ChatServer : public IOCPServer
{
public:
	ChatServer() = default;
	~ChatServer();

	void Init(const UINT32 maxClient);
	virtual void OnConnect(const UINT32 clientIndex_) override;
	virtual void OnClose(const UINT32 clientIndex_) override;
	virtual void OnReceive(const UINT32 clientIndex_, const UINT32 size_, char* pData_) override;

	bool Run(const UINT32 maxClient);
	void End();

private:
	PacketManager* mPacketManager;
	AccountDB* mAccountDB;
	RoomManager* mRoomManager;
	//RedisManager* mRedisMgr;
	UserManager* mUserManager;
};