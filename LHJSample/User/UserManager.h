#pragma once
#include <unordered_map>

#include "../ResultCode.h"
#include "User.h"

class PacketManager;
class AccountDB;
class RoomManager;

class UserManager
{
public:
	UserManager() = default;
	~UserManager() = default;

	void Init(const INT32 maxUserCount_, AccountDB* accountDB, RoomManager* roomManager);
	void SetSendPacketFunc(std::function<void(UINT32, UINT32, char*)> sendpacketfunc);
	INT32 GetCurrentUserCnt() { return mCurrentUserCnt; }
	INT32 GetMaxUserCnt() { return mMaxUserCnt; }
	void IncreaseUserCnt() { mCurrentUserCnt++; }
	void DecreaseUserCnt();

	RESULT_CODE AddUser(std::string& userID_, int clientIndex_);

	INT32 FindUserIndexByID(std::string& userID_);
	void DeleteUserInfo(User* user_);
	User* GetUserByConnIdx(INT32 clientIndex_) { return mUserObjPool[clientIndex_]; }
	RESULT_CODE CheckLoginable(UINT32 clientIndex_, std::string& userID, std::string& password);
	bool CreateAccount(std::string& userID, std::string& password);


private:
	INT32 mMaxUserCnt = 0;
	INT32 mCurrentUserCnt = 0;

	std::vector<User*> mUserObjPool; //vector·Î
	std::unordered_map<std::string, int> mUserIDDictionary;
	AccountDB* mAccountDB;
};