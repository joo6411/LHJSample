#include "ClientInfo.h"

ClientInfo::ClientInfo()
{
	ZeroMemory(&mRecvOverlappedEx, sizeof(stOverlappedEx));
	mSock = INVALID_SOCKET;
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
		printf("[에러] CreateIoCompletionPort()함수 실패: %d\n", GetLastError());
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
		printf("[에러] WSARecv()함수 실패 : %d\n", WSAGetLastError());
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
		printf("[에러] WSASend()함수 실패 : %d\n", WSAGetLastError());
		return false;
	}

	return true;
}

bool ClientInfo::SendIO()
{
	if (mSendPos <= 0 || mIsSending)
	{
		return true;
	}

	std::lock_guard<std::mutex> guard(mSendLock);

	mIsSending = true;

	CopyMemory(mSendingBuf, &mSendBuf[0], mSendPos);

	//Overlapped I/O을 위해 각 정보를 셋팅해 준다.
	mSendOverlappedEx.m_wsaBuf.len = mSendPos;
	mSendOverlappedEx.m_wsaBuf.buf = &mSendingBuf[0];
	mSendOverlappedEx.m_eOperation = IOOperation::SEND;

	DWORD dwRecvNumBytes = 0;
	int nRet = WSASend(mSock,
		&(mSendOverlappedEx.m_wsaBuf),
		1,
		&dwRecvNumBytes,
		0,
		(LPWSAOVERLAPPED) & (mSendOverlappedEx),
		NULL);

	//socket_error이면 client socket이 끊어진걸로 처리한다.
	if (nRet == SOCKET_ERROR && (WSAGetLastError() != ERROR_IO_PENDING))
	{
		printf("[에러] WSASend()함수 실패 : %d\n", WSAGetLastError());
		return false;
	}

	mSendPos = 0;
	return true;
}

void ClientInfo::SendCompleted(const UINT32 dataSize_)
{
	mIsSending = false;
	printf("[송신 완료] bytes : %d\n", dataSize_);
}