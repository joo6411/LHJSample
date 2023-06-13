#include "EchoServer.h"

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
}