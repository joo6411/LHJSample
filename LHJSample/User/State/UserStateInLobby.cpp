#include "UserStateInLobby.h"
#include "../User.h"
#include "../../ResultCode.h"
#include <iostream>


UserStateInLobby::UserStateInLobby(User* user) :
    UserStateBase(user)
{

}

UserStateInLobby::~UserStateInLobby()
{
}

void UserStateInLobby::Enter()
{
	mUser->ResetRecvFunction();
	mUser->recvFuntionDictionary[(int)PACKET_ID::REQ_LOBBY_INFO] = [this](UINT16 packetSize, char* packet) -> void { ProcessLobbyInfo(mUser->GetNetConnIdx(), packetSize, packet); };
	mUser->recvFuntionDictionary[(int)PACKET_ID::REQ_ROOM_ENTER] = [this](UINT16 packetSize, char* packet) -> void { ProcessEnterRoom(mUser->GetNetConnIdx(), packetSize, packet); };
}

void UserStateInLobby::Execute()
{
}

void UserStateInLobby::Exit()
{

}

void UserStateInLobby::ProcessLobbyInfo(UINT32 clientIndex_, UINT16 packetSize_, char* pPacket_)
{
	auto packet = reinterpret_cast<REQ_LOBBY_INFO*>(pPacket_);

	ACK_LOBBY_INFO lobbyInfoAckPacket;
	lobbyInfoAckPacket.PacketId = (UINT16)PACKET_ID::ACK_LOBBY_INFO;
	lobbyInfoAckPacket.PacketLength = sizeof(ACK_LOBBY_INFO);
	lobbyInfoAckPacket.Result = (UINT16)RESULT_CODE::LOBBY_INFO_SUCCESS;

	mUser->sendPacketFunc(clientIndex_, sizeof(ACK_LOBBY_INFO), (char*)&lobbyInfoAckPacket);

	NOTIFY_LOBBY_INFO lobbyInfoNtfPacket;
	lobbyInfoNtfPacket.Room = mUser->GetRoomList();

	int packetLength = PACKET_HEADER_LENGTH + 4 + lobbyInfoNtfPacket.Room.size() * 4;
	lobbyInfoNtfPacket.PacketId = (UINT16)PACKET_ID::NOTIFY_LOBBY_INFO;
	lobbyInfoNtfPacket.PacketLength = packetLength;
	lobbyInfoNtfPacket.RoomCount = lobbyInfoNtfPacket.Room.size();

	char* buffer = new char[packetLength];
	CopyMemory(buffer, &lobbyInfoNtfPacket, PACKET_HEADER_LENGTH + 4);
	if (lobbyInfoNtfPacket.Room.size() > 0)
	{
		CopyMemory(buffer + PACKET_HEADER_LENGTH + 4, &(*lobbyInfoNtfPacket.Room.begin()), lobbyInfoNtfPacket.Room.size() * 4);
	}
	mUser->sendPacketFunc(clientIndex_, packetLength, buffer);

	delete[] buffer;

	std::cout << ("ACK_LOBBY_INFO Sended") << std::endl;
}

void UserStateInLobby::ProcessEnterRoom(UINT32 clientIndex_, UINT16 packetSize_, char* pPacket_)
{
	auto roomEnterReqPacket = reinterpret_cast<REQ_ROOM_ENTER_PACKET*>(pPacket_);

	ACK_ROOM_ENTER_PACKET roomEnterAckPacket;
	roomEnterAckPacket.PacketId = (UINT16)PACKET_ID::ACK_ROOM_ENTER;
	roomEnterAckPacket.PacketLength = sizeof(ACK_ROOM_ENTER_PACKET);
	roomEnterAckPacket.Result = (UINT16)mUser->EnterRoom(roomEnterReqPacket->RoomNumber);

	mUser->sendPacketFunc(clientIndex_, sizeof(ACK_ROOM_ENTER_PACKET), (char*)&roomEnterAckPacket);
	std::cout << ("ACK_ROOM_ENTER_PACKET Sended");
}