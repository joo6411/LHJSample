#include "Room.h"

void Room::Init(const INT32 roomNum_, const INT32 maxUserCount_)
{
	mRoomNum = roomNum_;
	mMaxUserCount = maxUserCount_;
}

UINT16 Room::EnterUser(User* user_)
{
	if (mCurrentUserCount >= mMaxUserCount)
	{
		return (UINT16)ERROR_CODE::ENTER_ROOM_FULL_USER;
	}

	for (auto user : mUserList)
	{
		NOTIFY_ROOM_INFO_PACKET roomInfoNotPacket;
		roomInfoNotPacket.PacketId = (UINT16)PACKET_ID::NOTIFY_ROOM_INFO;
		roomInfoNotPacket.PacketLength = sizeof(NOTIFY_ROOM_INFO_PACKET);
		roomInfoNotPacket.Result = (UINT16)ERROR_CODE::NONE;
		roomInfoNotPacket.EnterUser = user_->GetUserId();
		SendPacketFunc(user->GetNetConnIdx(), sizeof(NOTIFY_ROOM_INFO_PACKET), (char*)&roomInfoNotPacket);
	}

	mUserList.push_back(user_);
	++mCurrentUserCount;

	user_->EnterRoom(mRoomNum);
	return (UINT16)ERROR_CODE::NONE;
}

void Room::LeaveUser(User* leaveUser_)
{
	mUserList.remove_if([leaveUserId = leaveUser_->GetUserId()](User* pUser) {
		return leaveUserId == pUser->GetUserId();
		});
}

void Room::NotifyChat(INT32 clientIndex_, const char* userID_, const char* msg_)
{
	NOTIFY_ROOM_CHAT_PACKET roomChatNtfyPkt;
	roomChatNtfyPkt.PacketId = (UINT16)PACKET_ID::NOTIFY_ROOM_CHAT;
	roomChatNtfyPkt.PacketLength = sizeof(roomChatNtfyPkt);

	CopyMemory(roomChatNtfyPkt.Msg, msg_, sizeof(roomChatNtfyPkt.Msg));
	CopyMemory(roomChatNtfyPkt.UserID, userID_, sizeof(roomChatNtfyPkt.UserID));
	SendToAllUser(sizeof(roomChatNtfyPkt), (char*)&roomChatNtfyPkt, clientIndex_, false);
}

void Room::SendToAllUser(const UINT16 dataSize_, char* data_, const INT32 passUserIndex_, bool exceptMe)
{
	for (auto pUser : mUserList)
	{
		if (pUser == nullptr) {
			continue;
		}

		if (exceptMe && pUser->GetNetConnIdx() == passUserIndex_) {
			continue;
		}

		SendPacketFunc((UINT32)pUser->GetNetConnIdx(), (UINT32)dataSize_, data_);
	}
}

std::list<std::string> Room::GetRoomMemberList()
{
	std::list<std::string> roomUser;
	for (auto user : mUserList)
	{
		roomUser.push_back(user->GetUserId());
	}

	return roomUser;
}