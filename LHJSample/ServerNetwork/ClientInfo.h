#pragma once

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
	void Close(bool bIsForce = false);
	void Clear(){}
	bool BindIOCompletionPort(HANDLE iocpHandle_);
	bool BindRecv();

	// 1���� �����忡���� ȣ���ؾ� �Ѵ�!
	bool SendMsg(const UINT32 dataSize_, char* pMsg_);
	void SendCompleted(const UINT32 dataSize_);
	bool SendIO();

private:
	INT32 mIndex = 0;
	SOCKET			mSock;			//Cliet�� ����Ǵ� ����
	stOverlappedEx	mRecvOverlappedEx;	//RECV Overlapped I/O�۾��� ���� ����

	char			mRecvBuf[MAX_SOCKBUF]; //������ ����	

	std::mutex mSendLock;
	std::queue<stOverlappedEx*> mSendDataqueue;
};