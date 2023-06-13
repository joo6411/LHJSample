#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

class IOCPClient {
public:
	IOCPClient();
	~IOCPClient();

    bool Run();
    bool Init();
    void Cleanup();

private:
    SOCKET m_hClientSock;
    SOCKADDR_IN m_ClientAddr;
};

void ErrorHandling(const char* message);