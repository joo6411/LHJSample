#pragma once

#include "ServerNetwork/IOCPServer.h"

class EchoServer : public IOCPServer
{
	virtual void OnConnect(const UINT32 clientIndex_) override;

	virtual void OnClose(const UINT32 clientIndex_) override;

	virtual void OnReceive(const UINT32 clientIndex_, const UINT32 size_, char* pData_) override;
};