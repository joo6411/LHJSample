#include "IOCPServer.h"
#include <iostream>

IOCPServer::IOCPServer()
{
}

IOCPServer::~IOCPServer()
{
    WSACleanup();
}

bool IOCPServer::InitSocket()
{
    WSADATA wsaData;

    int nRet = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (0 != nRet)
    {
		std::cout << "[����] WSAStartup()�Լ� ���� : " << WSAGetLastError() << std::endl;
        return false;
    }

    //���������� TCP , Overlapped I/O ������ ����
    mListenSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, NULL, WSA_FLAG_OVERLAPPED);

    if (INVALID_SOCKET == mListenSocket)
    {
		std::cout << "[����] socket()�Լ� ���� : " << WSAGetLastError() << std::endl;
        return false;
    }

    std::cout<<("���� �ʱ�ȭ ����\n");
    return true;
}

bool IOCPServer::BindandListen(int nBindPort)
{
    SOCKADDR_IN		stServerAddr;
    stServerAddr.sin_family = AF_INET;
    stServerAddr.sin_port = htons(nBindPort); //���� ��Ʈ�� �����Ѵ�.		
    //� �ּҿ��� ������ �����̶� �޾Ƶ��̰ڴ�.
    //���� ������� �̷��� �����Ѵ�. ���� �� �����ǿ����� ������ �ް� �ʹٸ�
    //�� �ּҸ� inet_addr�Լ��� �̿��� ������ �ȴ�.
    stServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    //������ ������ ���� �ּ� ������ cIOCPServer ������ �����Ѵ�.
    int nRet = bind(mListenSocket, (SOCKADDR*)&stServerAddr, sizeof(SOCKADDR_IN));
    if (0 != nRet)
    {
		std::cout << "[����] bind()�Լ� ���� : " << WSAGetLastError() << std::endl;
        return false;
    }

    //���� ��û�� �޾Ƶ��̱� ���� cIOCPServer������ ����ϰ� 
    //���Ӵ��ť�� 5���� ���� �Ѵ�.
    nRet = listen(mListenSocket, 5);
    if (0 != nRet)
    {
		std::cout << "[����] listen()�Լ� ���� : " << WSAGetLastError() << std::endl;
        return false;
    }

	//CompletionPort��ü ���� ��û�� �Ѵ�.
	mIOCPHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, MAX_WORKERTHREAD);
	if (NULL == mIOCPHandle)
	{
		std::cout << "[����] CreateIoCompletionPort()�Լ� ����: " << GetLastError() << std::endl;
		return false;
	}

	auto hIOCPHandle = CreateIoCompletionPort((HANDLE)mListenSocket, mIOCPHandle, (UINT32)0, 0);
	if (nullptr == hIOCPHandle)
	{
		std::cout << "[����] listen socket IOCP bind ���� : " << WSAGetLastError() << std::endl;
		return false;
	}

    std::cout<<("���� ��� ����..\n");
    return true;
}

//���� ��û�� �����ϰ� �޼����� �޾Ƽ� ó���ϴ� �Լ�
bool IOCPServer::StartServer(const UINT32 maxClientCount)
{
    CreateClient(maxClientCount);

    bool bRet = CreateWokerThread();
    if (false == bRet) {
        return false;
    }

    bRet = CreateAccepterThread();
    if (false == bRet) {
        return false;
    }

    std::cout<<("���� ����\n");
    return true;
}

//�����Ǿ��ִ� �����带 �ı��Ѵ�.
void IOCPServer::DestroyThread()
{
    mIsWorkerRun = false;
    CloseHandle(mIOCPHandle);

    for (auto& th : mIOWorkerThreads)
    {
        if (th.joinable())
        {
            th.join();
        }
    }

    //Accepter �����带 �����Ѵ�.
    mIsAccepterRun = false;
    closesocket(mListenSocket);

    if (mAccepterThread.joinable())
    {
        mAccepterThread.join();
    }
}

bool IOCPServer::SendMsg(const UINT32 sessionIndex_, const UINT32 dataSize_, char* pData)
{
	auto pClient = GetClientInfo(sessionIndex_);
	return pClient->SendMsg(dataSize_, pData);
}

void IOCPServer::CreateClient(const UINT32 maxClientCount)
{
	for (UINT32 i = 0; i < maxClientCount; ++i)
	{
		auto client = new ClientInfo();
		client->Init(i, mIOCPHandle);
		mClientInfos.push_back(client);
	}
}

//WaitingThread Queue���� ����� ��������� ����
bool IOCPServer::CreateWokerThread()
{
    unsigned int uiThreadId = 0;
    //WaingThread Queue�� ��� ���·� ���� ������� ���� ����Ǵ� ���� : (cpu���� * 2) + 1 
    for (int i = 0; i < MAX_WORKERTHREAD; i++)
    {
        mIOWorkerThreads.emplace_back([this]() { WokerThread(); });
    }

	std::cout << ("WokerThread ����..") << std::endl;
    return true;
}

bool IOCPServer::CreateAccepterThread()
{
    mAccepterThread = std::thread([this]() { AccepterThread(); });

	std::cout << ("AccepterThread ����..") << std::endl;
    return true;
}

void IOCPServer::CreateSendThread()
{
	mIsSenderRun = true;
	mSendThread = std::thread([this]() { SendThread(); });
	std::cout << ("SendThread ����..") << std::endl;
}

//������� �ʴ� Ŭ���̾�Ʈ ���� ����ü�� ��ȯ�Ѵ�.
ClientInfo* IOCPServer::GetEmptyClientInfo()
{
	for (auto& client : mClientInfos)
	{
		if (!client->IsConnected())
		{
			return client;
		}
	}

	return nullptr;
}

ClientInfo* IOCPServer::GetClientInfo(const UINT32 sessionIndex)
{
	return mClientInfos[sessionIndex];
}

//Overlapped I/O�۾��� ���� �Ϸ� �뺸�� �޾� 
//�׿� �ش��ϴ� ó���� �ϴ� �Լ�
void IOCPServer::WokerThread()
{
	//CompletionKey�� ���� ������ ����
	ClientInfo* pClientInfo = NULL;
	//�Լ� ȣ�� ���� ����
	BOOL bSuccess = TRUE;
	//Overlapped I/O�۾����� ���۵� ������ ũ��
	DWORD dwIoSize = 0;
	//I/O �۾��� ���� ��û�� Overlapped ����ü�� ���� ������
	LPOVERLAPPED lpOverlapped = NULL;

	while (mIsWorkerRun)
	{
		//////////////////////////////////////////////////////
		//�� �Լ��� ���� ��������� WaitingThread Queue��
		//��� ���·� ���� �ȴ�.
		//�Ϸ�� Overlapped I/O�۾��� �߻��ϸ� IOCP Queue����
		//�Ϸ�� �۾��� ������ �� ó���� �Ѵ�.
		//�׸��� PostQueuedCompletionStatus()�Լ������� �����
		//�޼����� �����Ǹ� �����带 �����Ѵ�.
		//////////////////////////////////////////////////////
		bSuccess = GetQueuedCompletionStatus(mIOCPHandle,
			&dwIoSize,					// ������ ���۵� ����Ʈ
			(PULONG_PTR)&pClientInfo,		// CompletionKey
			&lpOverlapped,				// Overlapped IO ��ü
			INFINITE);					// ����� �ð�

		//����� ������ ���� �޼��� ó��..
		if (TRUE == bSuccess && 0 == dwIoSize && NULL == lpOverlapped)
		{
			mIsWorkerRun = false;
			continue;
		}

		if (NULL == lpOverlapped)
		{
			continue;
		}

		stOverlappedEx* pOverlappedEx = (stOverlappedEx*)lpOverlapped;

		//client�� ������ ��������..			
		if (FALSE == bSuccess || (0 == dwIoSize && IOOperation::ACCEPT != pOverlappedEx->m_eOperation))
		{
			CloseSocket(pClientInfo);
			continue;
		}

		if (IOOperation::ACCEPT == pOverlappedEx->m_eOperation)
		{
			pClientInfo = GetClientInfo(pOverlappedEx->SessionIndex);
			if (pClientInfo->AcceptCompletion())
			{
				//Ŭ���̾�Ʈ ���� ����
				++mClientCnt;

				OnConnect(pClientInfo->GetIndex());
			}
			else
			{
				CloseSocket(pClientInfo, true);  //Caller WokerThread()
			}
		}
		//Overlapped I/O Recv�۾� ��� �� ó��
		else if (IOOperation::RECV == pOverlappedEx->m_eOperation)
		{
			OnReceive(pClientInfo->GetIndex(), dwIoSize, pClientInfo->RecvBuffer());
			pClientInfo->BindRecv();
		}
		//Overlapped I/O Send�۾� ��� �� ó��
		else if (IOOperation::SEND == pOverlappedEx->m_eOperation)
		{
			pClientInfo->SendCompleted(dwIoSize);
		}
		//���� ��Ȳ
		else
		{
			std::cout << "Client Index(" << pClientInfo->GetIndex() << ")���� ���ܻ�Ȳ" << std::endl;
		}
	}
}

//������� ������ �޴� ������
void IOCPServer::AccepterThread()
{
	while (mIsAccepterRun)
	{
		auto curTimeSec = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now().time_since_epoch()).count();

		for (auto client : mClientInfos)
		{
			if (client->IsConnected())
			{
				continue;
			}

			if ((UINT64)curTimeSec < client->GetLatestClosedTimeSec())
			{
				continue;
			}

			auto diff = curTimeSec - client->GetLatestClosedTimeSec();
			if (diff <= RE_USE_SESSION_WAIT_TIMESEC)
			{
				continue;
			}

			client->PostAccept(mListenSocket, curTimeSec);
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(32));
	}
}

//������ ������ ���� ��Ų��.
void IOCPServer::CloseSocket(ClientInfo* pClientInfo, bool bIsForce)
{
	auto clientIndex = pClientInfo->GetIndex();

	pClientInfo->Close(bIsForce);

	OnClose(clientIndex);
}

void IOCPServer::SendThread()
{
	while (mIsSenderRun)
	{
		for (auto client : mClientInfos)
		{
			if (client->IsConnected() == false)
			{
				continue;
			}

			client->SendIO();
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(8));
	}
}