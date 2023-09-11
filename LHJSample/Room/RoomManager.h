#pragma once
#include "Room.h"
#include <vector>

class RoomManager
{
public:
	RoomManager() = default;
	~RoomManager() = default;

	void Init(const INT32 beginRoomNumber_, const INT32 maxRoomCount_, const INT32 maxRoomUserCount_);
	RESULT_CODE EnterUser(INT32 roomNumber_, User* user_);
	RESULT_CODE LeaveUser(INT32 roomNumber_, User* user_);
	Room* GetRoomByNumber(INT32 number_);
	std::vector<int> GetRoomList();
	void SetSendPacketFunc(std::function<void(UINT32, UINT32, char*)> sendpacketfunc);

	std::function<void(UINT32, UINT16, char*)> SendPacketFunc;

private:
	std::vector<Room*> mRoomList;
	INT32 mBeginRoomNumber = 0;
	INT32 mEndRoomNumber = 0;
	INT32 mMaxRoomCount = 0;
};
