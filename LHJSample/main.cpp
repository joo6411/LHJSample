#include "ChatServer.h"
#include "Config/ConfigLoader.h"
#include "DB/AccountDB.h"
#include <string>
#include <iostream>

const UINT16 MAX_CLIENT = 100;		//총 접속할수 있는 클라이언트 수

int main()
{
	gConfig.LoadNetworkConfig("../Config.xml");

	ChatServer server;
	
	//소켓 초기화
	server.InitSocket();
	server.BindandListen(gConfig.GetChatServerInfo().Port);
	server.Init(MAX_CLIENT);

	if (!server.Run(MAX_CLIENT)) // 서버실행
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