#pragma once
#pragma comment(lib, "mswsock.lib")
#include "Define.h"
#include <stdio.h>
#include <mutex>
#include <queue>

//Ŭ���̾�Ʈ ������ ������� ����ü
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

	// 1���� �����忡���� ȣ���ؾ� �Ѵ�!
	bool SendMsg(const UINT32 dataSize_, char* pMsg_);
	void SendCompleted(const UINT32 dataSize_);
	bool SendIO();

	UINT64 GetLatestClosedTimeSec() { return mLatestClosedTimeSec; }

private:
	INT32 mIndex = 0;
	HANDLE mIOCPHandle = INVALID_HANDLE_VALUE;

	INT64 mIsConnect = 0;
	UINT64 mLatestClosedTimeSec = 0;

	SOCKET			mSock;			//Cliet�� ����Ǵ� ����

	stOverlappedEx	mAcceptContext;
	char mAcceptBuf[64];

	stOverlappedEx	mRecvOverlappedEx;	//RECV Overlapped I/O�۾��� ���� ����
	char			mRecvBuf[MAX_SOCKBUF]; //������ ����	

	std::mutex mSendLock;
	std::queue<stOverlappedEx*> mSendDataqueue;
};