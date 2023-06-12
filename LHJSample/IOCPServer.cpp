#include "IOCPServer.h"

IOCPServer::IOCPServer()
{
}

IOCPServer::~IOCPServer()
{
    closesocket(m_hServSock);
    WSACleanup();
}

bool IOCPServer::Init()
{
    WSAData wsaData;
    SYSTEM_INFO systemInfo;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {   // Load Winsock 2.2 DLL
        ErrorHandling("WSAStartup() error!");
    }

    // Completion Port ����
    m_hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
    GetSystemInfo(&systemInfo);

    // �����带 CPU ������ŭ ����
    for (int i = 0; i < systemInfo.dwNumberOfProcessors; i++)
    {
        _beginthreadex(NULL, 0, _CompletionThread, (LPVOID)this, 0, NULL);
    }

    setSocket();

    return true;
}

void IOCPServer::Cleanup()
{
}

bool IOCPServer::setSocket()
{
    // IOCP�� ����ϱ� ���� ���� ���� (Overlapped I/O ����, �������)
    m_hServSock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
    if (m_hServSock == INVALID_SOCKET)
    {
        ErrorHandling("WSASocket() error!");
    }

    // ���ε��� ���� �ּ�����
    memset(&m_servAddr, 0, sizeof(m_servAddr));
    m_servAddr.sin_family = AF_INET;
    m_servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    m_servAddr.sin_port = htons(PORT);

    if (bind(m_hServSock, (SOCKADDR*)&m_servAddr, sizeof(m_servAddr)) == SOCKET_ERROR)
    {
        ErrorHandling("bind() error!");
    }

    if (listen(m_hServSock, 10) == SOCKET_ERROR)
    {
        ErrorHandling("listen() error!");
    }

    return true;
}

bool IOCPServer::Run()
{
    LPPER_IO_DATA     perIoData;
    LPPER_HANDLE_DATA perHandleData;

    DWORD  dwRecvBytes;
    DWORD  i, dwFlags;

    while (TRUE)
    {
        SOCKET      hClntSock;
        SOCKADDR_IN clntAddr;
        int         nAddrLen = sizeof(clntAddr);

        hClntSock = accept(m_hServSock, (SOCKADDR*)&clntAddr, &nAddrLen);
        if (hClntSock == INVALID_SOCKET)
        {
            ErrorHandling("accept() error!");
        }

        // ����� Ŭ���̾�Ʈ�� ���� �ڵ� ������ �ּ� ������ ����
        perHandleData = new PER_HANDLE_DATA;
        perHandleData->hClntSock = hClntSock;
        memcpy(&(perHandleData->clntAddr), &clntAddr, nAddrLen);

        // 3. Overlapped ���ϰ� Completion Port �� ����
        CreateIoCompletionPort((HANDLE)hClntSock, m_hCompletionPort, (ULONG_PTR)perHandleData, 0);

        // Ŭ���̾�Ʈ�� ���� ���۸� ����, OVERLAPPED ���� �ʱ�ȭ
        perIoData = new PER_IO_DATA;
        memset(&(perIoData->overlapped), 0, sizeof(OVERLAPPED));
        perIoData->wsaBuf.len = BUFSIZE;
        perIoData->wsaBuf.buf = perIoData->buffer;
        dwFlags = 0;

        // 4. ��ø�� ����Ÿ �Է�
        WSARecv(perHandleData->hClntSock,       // ����Ÿ �Է� ����
            &(perIoData->wsaBuf),               // ����Ÿ �Է� ���� ������
            1,                                  // ����Ÿ �Է� ������ ��
            (LPDWORD)&dwRecvBytes,
            (LPDWORD)&dwFlags,
            &(perIoData->overlapped),           // OVERLAPPED ���� ������
            NULL
        );
    }

    return true;
}


unsigned int __stdcall IOCPServer::_CompletionThread(void* p_this)
{
    IOCPServer* p_IOCPServer = static_cast<IOCPServer*>(p_this);
    p_IOCPServer->CompletionThread(); // Non-static member function!
    return 0;
}

UINT WINAPI IOCPServer::CompletionThread()
{
    HANDLE hCompletionPort = (HANDLE)m_hCompletionPort;
    SOCKET hClntSock;
    DWORD  dwBytesTransferred;
    LPPER_HANDLE_DATA perHandleData;
    LPPER_IO_DATA     perIoData;
    DWORD  dwFlags;

    while (TRUE)
    {
        // 5. ��.����� �Ϸ�� ������ ���� ����
        GetQueuedCompletionStatus(hCompletionPort,      // Completion Port
            &dwBytesTransferred,    // ���۵� ����Ʈ ��
            (PULONG_PTR)&perHandleData,
            (LPOVERLAPPED*)&perIoData,
            INFINITE);

        if (dwBytesTransferred == 0)        // ���۵� ����Ʈ�� 0�϶� ���� (EOF ���� �ÿ���)
        {
            puts("socket will be closed!");
            closesocket(perHandleData->hClntSock);
            delete perHandleData;
            delete perIoData;
            continue;
        }

        perIoData->wsaBuf.buf[dwBytesTransferred] = '\0';
        printf("Recv[%s]\n", perIoData->wsaBuf.buf);

        // 6. �޼���. Ŭ���̾�Ʈ�� ����
        //memset(&(perIoData->overlapped), 0, sizeof(OVERLAPPED));
        perIoData->wsaBuf.len = dwBytesTransferred;
        DWORD bytes = 0;
        WSASend(perHandleData->hClntSock, &(perIoData->wsaBuf), 1, &bytes, 0, NULL, nullptr);

        // RECEIVE AGAIN
        memset(&(perIoData->overlapped), 0, sizeof(OVERLAPPED));
        perIoData->wsaBuf.len = BUFSIZE;
        perIoData->wsaBuf.buf = perIoData->buffer;
        dwFlags = 0;
        WSARecv(perHandleData->hClntSock, &(perIoData->wsaBuf), 1, NULL, &dwFlags, &(perIoData->overlapped), NULL);
    }

    return 0;
}


void ErrorHandling(LPCSTR pszMessage)
{
    fputs(pszMessage, stderr);
    fputc('\n', stderr);
    exit(1);
}
