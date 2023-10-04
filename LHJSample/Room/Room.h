#pragma once

#include "../User/UserManager.h"
#include "../Packet.h""

#include <functional>


class Room
{
public:
	Room() = default;
	~Room() = default;

	INT32 GetMaxUserCount() { return mMaxUserCount; }
	INT32 GetCurrentUserCount() { return mCurrentUserCount; }
	INT32 GetRoomNumber() { return mRoomNum; }
	std::vector<std::string> GetRoomMemberList();

	void Init(const INT32 roomNum_, const INT32 maxUserCount_);
	RESULT_CODE EnterUser(User* user_);
	RESULT_CODE LeaveUser(User* leaveUser_);
	void NotifyChat(INT32 clientIndex_, const char* userID_, const char* msg_);
	std::function<void(UINT32, UINT32, char*)> SendPacketFunc;

private:
	void SendToAllUser(const UINT16 dataSize_, char* data_, const INT32 passUserIndex_, bool exceptMe);
	
	INT32 mRoomNum = -1;
	std::list<User*> mUserList;
	INT32 mMaxUserCount = 0;
	UINT16 mCurrentUserCount = 0;
};
