#include <stdio.h>
#include <stdlib.h>
//#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

void ErrorHandling(const char* message);

int main()
{
    WSADATA wsaData;
    SOCKET hSocket;
    SOCKADDR_IN recvAddr;

    WSABUF dataBuf;
    char message[1024] = { 0, };
    int sendBytes = 0;
    int recvBytes = 0;
    int flags = 0;

    WSAEVENT event;
    WSAOVERLAPPED overlapped;


    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) /* Load Winsock 2.2 DLL */
        ErrorHandling("WSAStartup() error!");

    hSocket = WSASocket(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
    if (hSocket == INVALID_SOCKET)
        ErrorHandling("socket() error");

    memset(&recvAddr, 0, sizeof(recvAddr));
    recvAddr.sin_family = AF_INET;
    //recvAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    inet_pton(AF_INET, "127.0.0.1", &(recvAddr.sin_addr));
    recvAddr.sin_port = htons(5000);

    if (connect(hSocket, (SOCKADDR*)&recvAddr, sizeof(recvAddr)) == SOCKET_ERROR)
        ErrorHandling("connect() error!");


    //����ü���̺�Ʈ�ڵ�����ؼ�����
    event = WSACreateEvent();
    memset(&overlapped, 0, sizeof(overlapped));

    overlapped.hEvent = event;



    //�����ҵ�����
    while (true)
    {
        flags = 0;
        printf("�����ҵ�����(���Ḧ���ҽ�exit)\n:");
        scanf_s("%s", message, sizeof(message));

        if (!strcmp(message, "exit")) break;

        dataBuf.len = strlen(message);
        dataBuf.buf = message;

        if (WSASend(hSocket, &dataBuf, 1, (LPDWORD)&sendBytes, 0, &overlapped, NULL) == SOCKET_ERROR)
        {
            if (WSAGetLastError() != WSA_IO_PENDING)
                ErrorHandling("WSASend() error");
        }

        //���ۿϷ�Ȯ��
        WSAWaitForMultipleEvents(1, &event, TRUE, WSA_INFINITE, FALSE); //���������۳�������Ȯ��. 

        //���۵ȹ���Ʈ��Ȯ��
        WSAGetOverlappedResult(hSocket, &overlapped, (LPDWORD)&sendBytes, FALSE, (LPDWORD)&flags);//���� ���۵ȹ���Ʈ��������.
        printf("���۵ȹ���Ʈ��: %d \n", (int)sendBytes);
        if (WSARecv(hSocket, &dataBuf, 1, (LPDWORD)&recvBytes, (LPDWORD)&flags, &overlapped, NULL) == SOCKET_ERROR)
        {
            if (WSAGetLastError() != WSA_IO_PENDING)
                ErrorHandling("WSASend() error");
        }
        printf("Recv[%s]\n", dataBuf.buf);
    }

    closesocket(hSocket);
    WSACleanup();

    return 0;
}

void ErrorHandling(const char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}