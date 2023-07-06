#include "ClientInfo.h"
#include <mswsock.h>
#include <iostream>

ClientInfo::ClientInfo()
{
	ZeroMemory(&mRecvOverlappedEx, sizeof(stOverlappedEx));
	mSock = INVALID_SOCKET;
}

void ClientInfo::Init(const UINT32 index, HANDLE iocpHandle_)
{
	 mIndex = index; 
	 mIOCPHandle = iocpHandle_;
}

bool ClientInfo::OnConnect(HANDLE iocpHandle_, SOCKET socket_)
{
	mSock = socket_;

	Clear();

	//I/O Completion Port객체와 소켓을 연결시킨다.
	if (BindIOCompletionPort(iocpHandle_) == false)
	{
		return false;
	}

	return BindRecv();
}

bool ClientInfo::PostAccept(SOCKET listenSock_, const UINT64 curTimeSec_)
{
	mLatestClosedTimeSec = UINT32_MAX;

	mSock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (INVALID_SOCKET == mSock)
	{
		std::cout << "client Socket WSASocket Error : " << GetLastError() << std::endl;
		return false;
	}

	ZeroMemory(&mAcceptContext, sizeof(stOverlappedEx));

	DWORD bytes = 0;
	DWORD flags = 0;
	mAcceptContext.m_wsaBuf.len = 0;
	mAcceptContext.m_wsaBuf.buf = nullptr;
	mAcceptContext.m_eOperation = IOOperation::ACCEPT;
	mAcceptContext.SessionIndex = mIndex;

	if (FALSE == AcceptEx(listenSock_, mSock, mAcceptBuf, 0,
		sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, &bytes, (LPWSAOVERLAPPED) & (mAcceptContext)))
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			std::cout << "AcceptEx Error : " << GetLastError() << std::endl;
			return false;
		}
	}

	return true;
}

bool ClientInfo::AcceptCompletion()
{
	std::cout << "AcceptCompletion : SessionIndex(" << mIndex << ")" << std::endl;
	if (OnConnect(mIOCPHandle, mSock) == false)
	{
		return false;
	}

	SOCKADDR_IN		stClientAddr;
	int nAddrLen = sizeof(SOCKADDR_IN);
	char clientIP[32] = { 0, };
	inet_ntop(AF_INET, &(stClientAddr.sin_addr), clientIP, 32 - 1);
	std::cout << "클라이언트 접속 : IP(" << clientIP << ") SOCKET(" << (int)mSock << ")" << std::endl;

	return true;
}

void ClientInfo::Close(bool bIsForce)
{
	struct linger stLinger = { 0, 0 };	// SO_DONTLINGER로 설정

	if (true == bIsForce)
	{
		stLinger.l_onoff = 1;
	}

	shutdown(mSock, SD_BOTH);

	setsockopt(mSock, SOL_SOCKET, SO_LINGER, (char*)&stLinger, sizeof(stLinger));

	closesocket(mSock);
	mSock = INVALID_SOCKET;
}

bool ClientInfo::BindIOCompletionPort(HANDLE iocpHandle_)
{
	auto hIOCP = CreateIoCompletionPort((HANDLE)GetSock()
		, iocpHandle_
		, (ULONG_PTR)(this), 0);

	if (hIOCP == INVALID_HANDLE_VALUE)
	{
		std::cout << "[에러] CreateIoCompletionPort()함수 실패: " << GetLastError() << std::endl;
		return false;
	}

	return true;
}

bool ClientInfo::BindRecv()
{
	DWORD dwFlag = 0;
	DWORD dwRecvNumBytes = 0;

	mRecvOverlappedEx.m_wsaBuf.len = MAX_SOCKBUF;
	mRecvOverlappedEx.m_wsaBuf.buf = mRecvBuf;
	mRecvOverlappedEx.m_eOperation = IOOperation::RECV;

	int nRet = WSARecv(mSock,
		&(mRecvOverlappedEx.m_wsaBuf),
		1,
		&dwRecvNumBytes,
		&dwFlag,
		(LPWSAOVERLAPPED) & (mRecvOverlappedEx),
		NULL);

	//socket_error이면 client socket이 끊어진걸로 처리한다.
	if (nRet == SOCKET_ERROR && (WSAGetLastError() != ERROR_IO_PENDING))
	{
		std::cout << "[에러] WSARecv()함수 실패 : " << WSAGetLastError() << std::endl;
		return false;
	}

	return true;
}

// 1개의 스레드에서만 호출해야 한다!
bool ClientInfo::SendMsg(const UINT32 dataSize_, char* pMsg_)
{
	auto sendOverlappedEx = new stOverlappedEx;
	ZeroMemory(sendOverlappedEx, sizeof(stOverlappedEx));
	sendOverlappedEx->m_wsaBuf.len = dataSize_;
	sendOverlappedEx->m_wsaBuf.buf = new char[dataSize_];
	CopyMemory(sendOverlappedEx->m_wsaBuf.buf, pMsg_, dataSize_);
	sendOverlappedEx->m_eOperation = IOOperation::SEND;

	std::lock_guard<std::mutex> guard(mSendLock);

	mSendDataqueue.push(sendOverlappedEx);

	if (mSendDataqueue.size() == 1)
	{
		SendIO();
	}

	return true;
}

bool ClientInfo::SendIO()
{
	auto sendOverlappedEx = mSendDataqueue.front();

	DWORD dwRecvNumBytes = 0;
	int nRet = WSASend(mSock,
		&(sendOverlappedEx->m_wsaBuf),
		1,
		&dwRecvNumBytes,
		0,
		(LPWSAOVERLAPPED)sendOverlappedEx,
		NULL);

	//socket_error이면 client socket이 끊어진걸로 처리한다.
	if (nRet == SOCKET_ERROR && (WSAGetLastError() != ERROR_IO_PENDING))
	{
		std::cout << "[에러] WSASend()함수 실패 : " << WSAGetLastError() << std::endl;
		return false;
	}

	return true;
}

void ClientInfo::SendCompleted(const UINT32 dataSize_)
{
	std::cout << "[송신 완료] bytes : " << dataSize_ << std::endl;

	std::lock_guard<std::mutex> guard(mSendLock);

	delete[] mSendDataqueue.front()->m_wsaBuf.buf;
	delete mSendDataqueue.front();

	mSendDataqueue.pop();

	if (mSendDataqueue.empty() == false)
	{
		SendIO();
	}
}