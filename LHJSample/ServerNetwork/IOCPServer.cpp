#include "IOCPServer.h"

IOCPServer::IOCPServer()
{
}

IOCPServer::~IOCPServer()
{
    //������ ����� ������.
    WSACleanup();
}

bool IOCPServer::InitSocket()
{
    WSADATA wsaData;

    int nRet = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (0 != nRet)
    {
        printf("[����] WSAStartup()�Լ� ���� : %d\n", WSAGetLastError());
        return false;
    }

    //���������� TCP , Overlapped I/O ������ ����
    mListenSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, NULL, WSA_FLAG_OVERLAPPED);

    if (INVALID_SOCKET == mListenSocket)
    {
        printf("[����] socket()�Լ� ���� : %d\n", WSAGetLastError());
        return false;
    }

    printf("���� �ʱ�ȭ ����\n");
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
        printf("[����] bind()�Լ� ���� : %d\n", WSAGetLastError());
        return false;
    }

    //���� ��û�� �޾Ƶ��̱� ���� cIOCPServer������ ����ϰ� 
    //���Ӵ��ť�� 5���� ���� �Ѵ�.
    nRet = listen(mListenSocket, 5);
    if (0 != nRet)
    {
        printf("[����] listen()�Լ� ���� : %d\n", WSAGetLastError());
        return false;
    }

    printf("���� ��� ����..\n");
    return true;
}

//���� ��û�� �����ϰ� �޼����� �޾Ƽ� ó���ϴ� �Լ�
bool IOCPServer::StartServer(const UINT32 maxClientCount)
{
    CreateClient(maxClientCount);

    mIOCPHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, MAX_WORKERTHREAD);
    if (NULL == mIOCPHandle)
    {
        printf("[����] CreateIOCPServer()�Լ� ����: %d\n", GetLastError());
        return false;
    }

    bool bRet = CreateWokerThread();
    if (false == bRet) {
        return false;
    }

    bRet = CreateAccepterThread();
    if (false == bRet) {
        return false;
    }

    printf("���� ����\n");
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
		mClientInfos.emplace_back();
		mClientInfos[i].Init(i);
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

    printf("WokerThread ����..\n");
    return true;
}

bool IOCPServer::CreateAccepterThread()
{
    mAccepterThread = std::thread([this]() { AccepterThread(); });

    printf("AccepterThread ����..\n");
    return true;
}

//������� �ʴ� Ŭ���̾�Ʈ ���� ����ü�� ��ȯ�Ѵ�.
ClientInfo* IOCPServer::GetEmptyClientInfo()
{
	for (auto& client : mClientInfos)
	{
		if (!client.IsConnectd())
		{
			return &client;
		}
	}

	return nullptr;
}

ClientInfo* IOCPServer::GetClientInfo(const UINT32 sessionIndex)
{
	return &mClientInfos[sessionIndex];
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

		//client�� ������ ��������..			
		if (FALSE == bSuccess || (0 == dwIoSize && TRUE == bSuccess))
		{
			//printf("socket(%d) ���� ����\n", (int)pClientInfo->m_socketClient);
			CloseSocket(pClientInfo);
			continue;
		}


		stOverlappedEx* pOverlappedEx = (stOverlappedEx*)lpOverlapped;

		//Overlapped I/O Recv�۾� ��� �� ó��
		if (IOOperation::RECV == pOverlappedEx->m_eOperation)
		{
			OnReceive(pClientInfo->GetIndex(), dwIoSize, pClientInfo->RecvBuffer());
			pClientInfo->BindRecv();
		}
		//Overlapped I/O Send�۾� ��� �� ó��
		else if (IOOperation::SEND == pOverlappedEx->m_eOperation)
		{
			delete[] pOverlappedEx->m_wsaBuf.buf;
			delete pOverlappedEx;
			pClientInfo->SendCompleted(dwIoSize);
		}
		//���� ��Ȳ
		else
		{
			printf("Client Index(%d)���� ���ܻ�Ȳ\n", pClientInfo->GetIndex());
		}
	}
}

//������� ������ �޴� ������
void IOCPServer::AccepterThread()
{
	SOCKADDR_IN		stClientAddr;
	int nAddrLen = sizeof(SOCKADDR_IN);

	while (mIsAccepterRun)
	{
		//������ ���� ����ü�� �ε����� ���´�.
		ClientInfo* pClientInfo = GetEmptyClientInfo();
		if (NULL == pClientInfo)
		{
			printf("[����] Client Full\n");
			return;
		}

		//Ŭ���̾�Ʈ ���� ��û�� ���� ������ ��ٸ���.
		auto newSocket = accept(mListenSocket, (SOCKADDR*)&stClientAddr, &nAddrLen);
		if (INVALID_SOCKET == newSocket)
		{
			continue;
		}

		if (pClientInfo->OnConnect(mIOCPHandle, newSocket) == false)
		{
			pClientInfo->Close(true);
			return;
		}

		/*
		char clientIP[32] = { 0, };
		inet_ntop(AF_INET, &(stClientAddr.sin_addr), clientIP, 32 - 1);
		printf("Ŭ���̾�Ʈ ���� : IP(%s) SOCKET(%d)\n", clientIP, (int)pClientInfo->m_socketClient);
		*/

		OnConnect(pClientInfo->GetIndex());

		//Ŭ���̾�Ʈ ���� ����
		++mClientCnt;
	}
}

//������ ������ ���� ��Ų��.
void IOCPServer::CloseSocket(ClientInfo* pClientInfo, bool bIsForce)
{
	auto clientIndex = pClientInfo->GetIndex();

	pClientInfo->Close(bIsForce);

	OnClose(clientIndex);
}