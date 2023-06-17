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
	ROOM_CHAT_NOTIFY_PACKET roomChatNtfyPkt;
	roomChatNtfyPkt.PacketId = (UINT16)PACKET_ID::ROOM_CHAT_NOTIFY;
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