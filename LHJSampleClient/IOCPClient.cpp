#include "IOCPClient.h"

IOCPClient::IOCPClient()
{
}

IOCPClient::~IOCPClient()
{
    closesocket(m_hClientSock);
    WSACleanup();
}

bool IOCPClient::Init()
{
    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) /* Load Winsock 2.2 DLL */
        ErrorHandling("WSAStartup() error!");

    m_hClientSock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
    if (m_hClientSock == INVALID_SOCKET)
        ErrorHandling("socket() error");

    memset(&m_ClientAddr, 0, sizeof(m_ClientAddr));
    m_ClientAddr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &(m_ClientAddr.sin_addr));
    m_ClientAddr.sin_port = htons(5000);

    return true;
}

bool IOCPClient::Run()
{
    WSABUF dataBuf;
    WSAOVERLAPPED overlapped;
    WSAEVENT event;
    char message[1024] = { 0, };
    int sendBytes = 0;
    int recvBytes = 0;
    int flags = 0;

    if (connect(m_hClientSock, (SOCKADDR*)&m_ClientAddr, sizeof(m_ClientAddr)) == SOCKET_ERROR)
        ErrorHandling("connect() error!");

    //구조체에이벤트핸들삽입해서전달
    event = WSACreateEvent();
    memset(&overlapped, 0, sizeof(overlapped));
    overlapped.hEvent = event;


    //전송할데이터
    while (true)
    {
        flags = 0;
        printf("전송할데이터(종료를원할시exit)\n:");
        scanf_s("%s", message, sizeof(message));

        if (!strcmp(message, "exit")) break;

        dataBuf.len = strlen(message);
        dataBuf.buf = message;

        if (WSASend(m_hClientSock, &dataBuf, 1, (LPDWORD)&sendBytes, 0, &overlapped, NULL) == SOCKET_ERROR)
        {
            if (WSAGetLastError() != WSA_IO_PENDING)
                ErrorHandling("WSASend() error");
        }

        //전송완료확인
        WSAWaitForMultipleEvents(1, &event, TRUE, WSA_INFINITE, FALSE); //데이터전송끝났는지확인. 

        //전송된바이트수확인
        WSAGetOverlappedResult(m_hClientSock, &overlapped, (LPDWORD)&sendBytes, FALSE, (LPDWORD)&flags);//실제 전송된바이트수를얻어낸다.
        printf("전송된바이트수: %d \n", (int)sendBytes);
        if (WSARecv(m_hClientSock, &dataBuf, 1, (LPDWORD)&recvBytes, (LPDWORD)&flags, &overlapped, NULL) == SOCKET_ERROR)
        {
            if (WSAGetLastError() != WSA_IO_PENDING)
                ErrorHandling("WSASend() error");
        }
        printf("Recv[%s]\n", dataBuf.buf);
    }

    return true;
}

void IOCPClient::Cleanup()
{
}

void ErrorHandling(const char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}