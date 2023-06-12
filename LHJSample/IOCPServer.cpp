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

    // Completion Port 생성
    m_hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
    GetSystemInfo(&systemInfo);

    // 쓰레드를 CPU 개수만큼 생성
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
    // IOCP를 사용하기 위한 소켓 생성 (Overlapped I/O 소켓, 윈도우용)
    m_hServSock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
    if (m_hServSock == INVALID_SOCKET)
    {
        ErrorHandling("WSASocket() error!");
    }

    // 바인딩할 소켓 주소정보
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

        // 연결된 클라이언트의 소켓 핸들 정보와 주소 정보를 설정
        perHandleData = new PER_HANDLE_DATA;
        perHandleData->hClntSock = hClntSock;
        memcpy(&(perHandleData->clntAddr), &clntAddr, nAddrLen);

        // 3. Overlapped 소켓과 Completion Port 의 연결
        CreateIoCompletionPort((HANDLE)hClntSock, m_hCompletionPort, (ULONG_PTR)perHandleData, 0);

        // 클라이언트를 위한 버퍼를 설정, OVERLAPPED 변수 초기화
        perIoData = new PER_IO_DATA;
        memset(&(perIoData->overlapped), 0, sizeof(OVERLAPPED));
        perIoData->wsaBuf.len = BUFSIZE;
        perIoData->wsaBuf.buf = perIoData->buffer;
        dwFlags = 0;

        // 4. 중첩된 데이타 입력
        WSARecv(perHandleData->hClntSock,       // 데이타 입력 소켓
            &(perIoData->wsaBuf),               // 데이타 입력 버퍼 포인터
            1,                                  // 데이타 입력 버퍼의 수
            (LPDWORD)&dwRecvBytes,
            (LPDWORD)&dwFlags,
            &(perIoData->overlapped),           // OVERLAPPED 변수 포인터
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
        // 5. 입.출력이 완료된 소켓의 정보 얻음
        GetQueuedCompletionStatus(hCompletionPort,      // Completion Port
            &dwBytesTransferred,    // 전송된 바이트 수
            (PULONG_PTR)&perHandleData,
            (LPOVERLAPPED*)&perIoData,
            INFINITE);

        if (dwBytesTransferred == 0)        // 전송된 바이트가 0일때 종료 (EOF 전송 시에도)
        {
            puts("socket will be closed!");
            closesocket(perHandleData->hClntSock);
            delete perHandleData;
            delete perIoData;
            continue;
        }

        perIoData->wsaBuf.buf[dwBytesTransferred] = '\0';
        printf("Recv[%s]\n", perIoData->wsaBuf.buf);

        // 6. 메세지. 클라이언트로 에코
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
