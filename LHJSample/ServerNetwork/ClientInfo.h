#pragma once

#include "Define.h"
#include <stdio.h>
#include <mutex>
#include <queue>


//클라이언트 정보를 담기위한 구조체
class ClientInfo
{
public:
	ClientInfo();
	~ClientInfo() {}
	void Init(const UINT32 index) { mIndex = index; }
	UINT32 GetIndex() { return mIndex; }
	bool IsConnectd() { return mSock != INVALID_SOCKET; }
	SOCKET GetSock() { return mSock; }
	char* RecvBuffer() { return mRecvBuf; }

	bool OnConnect(HANDLE iocpHandle_, SOCKET socket_);
	void Close(bool bIsForce = false);
	void Clear(){}
	bool BindIOCompletionPort(HANDLE iocpHandle_);
	bool BindRecv();

	// 1개의 스레드에서만 호출해야 한다!
	bool SendMsg(const UINT32 dataSize_, char* pMsg_);
	void SendCompleted(const UINT32 dataSize_);
	bool SendIO();

private:
	INT32 mIndex = 0;
	SOCKET			mSock;			//Cliet와 연결되는 소켓
	stOverlappedEx	mRecvOverlappedEx;	//RECV Overlapped I/O작업을 위한 변수

	char			mRecvBuf[MAX_SOCKBUF]; //데이터 버퍼	

	std::mutex mSendLock;
	std::queue<stOverlappedEx*> mSendDataqueue;
};