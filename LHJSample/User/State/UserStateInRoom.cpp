#include "UserStateInRoom.h"
#include "../User.h"
#include "../../ResultCode.h"
#include "../../Room/RoomManager.h"
#include <iostream>

UserStateInRoom::UserStateInRoom(User* user) :
    UserStateBase(user)
{

}

UserStateInRoom::~UserStateInRoom()
{
}

void UserStateInRoom::Enter()
{
	mUser->ResetRecvFunction();
	mUser->recvFuntionDictionary[(int)PACKET_ID::REQ_ROOM_INFO] = [this](UINT16 packetSize, char* packet) -> void { ProcessRoomInfo(mUser->GetNetConnIdx(), packetSize, packet); };
	mUser->recvFuntionDictionary[(int)PACKET_ID::REQ_ROOM_LEAVE] = [this](UINT16 packetSize, char* packet) -> void { ProcessLeaveRoom(mUser->GetNetConnIdx(), packetSize, packet); };
	mUser->recvFuntionDictionary[(int)PACKET_ID::REQ_ROOM_CHAT] = [this](UINT16 packetSize, char* packet) -> void { ProcessRoomChatMessage(mUser->GetNetConnIdx(), packetSize, packet); };
}

void UserStateInRoom::Execute()
{
}

void UserStateInRoom::Exit()
{

}

void UserStateInRoom::ProcessRoomInfo(UINT32 clientIndex_, UINT16 packetSize_, char* pPacket_)
{
	Room* room = mUser->GetRoom();

	ACK_ROOM_INFO_PACKET roomInfoAckPacket;
	roomInfoAckPacket.PacketId = (UINT16)PACKET_ID::ACK_ROOM_INFO;
	roomInfoAckPacket.Result = (UINT16)RESULT_CODE::ENTER_ROOM_SUCCESS;
	roomInfoAckPacket.Users = room->GetRoomMemberList();
	roomInfoAckPacket.UserCount = room->GetRoomMemberList().size();
	int vectorLength = 0;
	for (int i = 0; i < roomInfoAckPacket.Users.size(); i++)
	{
		vectorLength += roomInfoAckPacket.Users[i].size() + 1;
	}

	int packetLength = PACKET_HEADER_LENGTH + 2 + 4 + vectorLength;
	roomInfoAckPacket.PacketLength = packetLength;

	char* buffer = new char[packetLength];
	CopyMemory(buffer, &roomInfoAckPacket, PACKET_HEADER_LENGTH + 2 + 4);
	int pos = 0;

	if (roomInfoAckPacket.Users.size() > 0)
	{
		for (int i = 0; i < roomInfoAckPacket.Users.size(); i++)
		{
			for (int j = 0; j < roomInfoAckPacket.Users[i].size(); j++)
			{
				buffer[PACKET_HEADER_LENGTH + 2 + 4 + pos] = roomInfoAckPacket.Users[i][j];
				pos++;
			}
			buffer[PACKET_HEADER_LENGTH + 2 + 4 + pos] = '\0';
			pos++;
		}
	}

	mUser->sendPacketFunc(clientIndex_, packetLength, buffer);
	delete[] buffer;

	std::cout << ("ACK_ROOM_INFO_PACKET Sended");
}


void UserStateInRoom::ProcessLeaveRoom(UINT32 clientIndex_, UINT16 packetSize_, char* pPacket_)
{
	ACK_ROOM_LEAVE_PACKET roomLeaveResPacket;
	roomLeaveResPacket.PacketId = (UINT16)PACKET_ID::ACK_ROOM_LEAVE;
	roomLeaveResPacket.PacketLength = sizeof(ACK_ROOM_LEAVE_PACKET);
	
	//TODO Room안의 UserList객체의 값 확인하기
	roomLeaveResPacket.Result = (UINT16)mUser->LeaveRoom();
	mUser->sendPacketFunc(clientIndex_, sizeof(ACK_ROOM_LEAVE_PACKET), (char*)&roomLeaveResPacket);
}

void UserStateInRoom::ProcessRoomChatMessage(UINT32 clientIndex_, UINT16 packetSize_, char* pPacket_)
{
	auto roomChatReqPacketet = reinterpret_cast<REQ_ROOM_CHAT_PACKET*>(pPacket_);

	ACK_ROOM_CHAT_PACKET roomChatAckPacket;
	roomChatAckPacket.PacketId = (UINT16)PACKET_ID::ACK_ROOM_CHAT;
	roomChatAckPacket.PacketLength = sizeof(ACK_ROOM_CHAT_PACKET);
	roomChatAckPacket.Result = (INT16)RESULT_CODE::CHAT_ROOM_SUCCESS;

	Room* room = mUser->GetRoom();
	if (room == nullptr)
	{
		roomChatAckPacket.Result = (INT16)RESULT_CODE::CHAT_ROOM_INVALID_ROOM_NUMBER;
		mUser->sendPacketFunc(clientIndex_, sizeof(ACK_ROOM_CHAT_PACKET), (char*)&roomChatAckPacket);
		return;
	}

	mUser->sendPacketFunc(clientIndex_, sizeof(ACK_ROOM_CHAT_PACKET), (char*)&roomChatAckPacket);

	room->NotifyChat(clientIndex_, mUser->GetUserId().c_str(), roomChatReqPacketet->Message);
}