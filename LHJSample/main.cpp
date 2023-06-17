#include "ChatServer.h"
#include "Config/ConfigLoader.h"
#include <string>
#include <iostream>

const UINT16 MAX_CLIENT = 100;		//총 접속할수 있는 클라이언트 수

int main()
{
	ChatServer server;
	gConfig.LoadNetworkConfig("../Config.xml");

	//소켓을 초기화
	server.InitSocket();

	//소켓과 서버 주소를 연결하고 등록 시킨다.
	server.BindandListen(gConfig.mChatServerInfo.Port);

	if (!server.Run(MAX_CLIENT))
	{
		std::cout << ("서버 실행 오류");
		server.DestroyThread();
		return 0;
	}
	
	std::cout<<("quit 입력 시까지 대기합니다\n");

	while (true)
	{
		std::string inputCmd;
		std::getline(std::cin, inputCmd);

		if (inputCmd == "quit")
		{
			break;
		}
	}

	server.DestroyThread();
	return 0;
}