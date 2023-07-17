#include "RoomManager.h"


void RoomManager::Init(const INT32 beginRoomNumber_, const INT32 maxRoomCount_, const INT32 maxRoomUserCount_)
{
	mBeginRoomNumber = beginRoomNumber_;
	mMaxRoomCount = maxRoomCount_;
	mEndRoomNumber = beginRoomNumber_ + maxRoomCount_;

	mRoomList = std::vector<Room*>(maxRoomCount_);

	for (auto i = 0; i < maxRoomCount_; i++)
	{
		mRoomList[i] = new Room();
		mRoomList[i]->SendPacketFunc = SendPacketFunc;
		mRoomList[i]->Init((i + beginRoomNumber_), maxRoomUserCount_);
	}
}

UINT16 RoomManager::EnterUser(INT32 roomNumber_, User* user_)
{
	auto room = GetRoomByNumber(roomNumber_);
	if (room == nullptr)
	{
		return (UINT16)RESULT_CODE::ROOM_INVALID_INDEX;
	}
	if (room->GetCurrentUserCount() >= room->GetMaxUserCount())
	{
		return (UINT16)RESULT_CODE::ROOM_FULL;
	}

	return room->EnterUser(user_);
}

INT16 RoomManager::LeaveUser(INT32 roomNumber_, User* user_)
{
	auto room = GetRoomByNumber(roomNumber_);
	if (room == nullptr)
	{
		return (INT16)RESULT_CODE::ROOM_INVALID_INDEX;
	}

	user_->SetDomainState(User::DOMAIN_STATE::LOGIN);
	room->LeaveUser(user_);
	return (INT16)RESULT_CODE::NONE;
}

Room* RoomManager::GetRoomByNumber(INT32 number_)
{
	if (number_ < mBeginRoomNumber || number_ >= mEndRoomNumber)
	{
		return nullptr;
	}

	auto index = (number_ - mBeginRoomNumber);
	return mRoomList[index];
}

std::vector<int> RoomManager::GetRoomList()
{
	std::vector<int> result;
	for (auto it = mRoomList.begin(); it != mRoomList.end(); it++)
	{
		result.push_back((*it)->GetRoomNumber());
	}

	return result;
}