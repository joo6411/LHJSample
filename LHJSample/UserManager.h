#pragma once
#include <unordered_map>

#include "User.h"

class UserManager
{
public:
	UserManager() = default;
	~UserManager() = default;

	void Init(const INT32 maxUserCount_);
	INT32 GetCurrentUserCnt() { return mCurrentUserCnt; }
	INT32 GetMaxUserCnt() { return mMaxUserCnt; }
	void IncreaseUserCnt() { mCurrentUserCnt++; }
	void DecreaseUserCnt();

	ERROR_CODE AddUser(char* userID_, int clientIndex_);

	INT32 FindUserIndexByID(char* userID_);
	void DeleteUserInfo(User* user_);
	User* GetUserByConnIdx(INT32 clientIndex_) { return mUserObjPool[clientIndex_]; }

private:
	INT32 mMaxUserCnt = 0;
	INT32 mCurrentUserCnt = 0;

	std::vector<User*> mUserObjPool; //vector·Î
	std::unordered_map<std::string, int> mUserIDDictionary;
};