#pragma once
#pragma comment(lib, "mswsock.lib")
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
	bool PostAccept(SOCKET listenSock_, const UINT64 curTimeSec_);
	bool AcceptCompletion();
	void Close(bool bIsForce = false);
	void Clear(){}
	bool BindIOCompletionPort(HANDLE iocpHandle_);
	bool BindRecv();

	// 1개의 스레드에서만 호출해야 한다!
	bool SendMsg(const UINT32 dataSize_, char* pMsg_);
	void SendCompleted(const UINT32 dataSize_);
	bool SendIO();

	UINT64 GetLatestClosedTimeSec() { return mLatestClosedTimeSec; }

private:
	INT32 mIndex = 0;
	HANDLE mIOCPHandle = INVALID_HANDLE_VALUE;

	INT64 mIsConnect = 0;
	UINT64 mLatestClosedTimeSec = 0;

	SOCKET			mSock;			//Cliet와 연결되는 소켓

	stOverlappedEx	mAcceptContext;
	char mAcceptBuf[64];

	stOverlappedEx	mRecvOverlappedEx;	//RECV Overlapped I/O작업을 위한 변수
	char			mRecvBuf[MAX_SOCKBUF]; //데이터 버퍼	

	std::mutex mSendLock;
	std::queue<stOverlappedEx*> mSendDataqueue;
};