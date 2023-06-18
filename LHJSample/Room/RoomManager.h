#pragma once
#include "Room.h"

class RoomManager
{
public:
	RoomManager() = default;
	~RoomManager() = default;

	void Init(const INT32 beginRoomNumber_, const INT32 maxRoomCount_, const INT32 maxRoomUserCount_);
	UINT16 EnterUser(INT32 roomNumber_, User* user_);
	INT16 LeaveUser(INT32 roomNumber_, User* user_);
	Room* GetRoomByNumber(INT32 number_);

	std::function<void(UINT32, UINT16, char*)> SendPacketFunc;

private:
	std::vector<Room*> mRoomList;
	INT32 mBeginRoomNumber = 0;
	INT32 mEndRoomNumber = 0;
	INT32 mMaxRoomCount = 0;
};