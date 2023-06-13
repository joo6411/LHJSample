#include "EchoServer.h"

void EchoServer::OnConnect(const UINT32 clientIndex_)
{
	printf("[OnConnect] Ŭ���̾�Ʈ: Index(%d)\n", clientIndex_);
}

void EchoServer::OnClose(const UINT32 clientIndex_) 
{
	printf("[OnClose] Ŭ���̾�Ʈ: Index(%d)\n", clientIndex_);
}

void EchoServer::OnReceive(const UINT32 clientIndex_, const UINT32 size_, char* pData_)
{
	printf("[OnReceive] Ŭ���̾�Ʈ: Index(%d), dataSize(%d)\n", clientIndex_, size_);
}