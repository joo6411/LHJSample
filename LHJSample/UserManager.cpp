#include "UserManager.h"

void UserManager::Init(const INT32 maxUserCount_)
{
	mMaxUserCnt = maxUserCount_;
	mUserObjPool = std::vector<User*>(mMaxUserCnt);

	for (auto i = 0; i < mMaxUserCnt; i++)
	{
		mUserObjPool[i] = new User();
		mUserObjPool[i]->Init(i);
	}
}

void UserManager::DecreaseUserCnt()
{
	if (mCurrentUserCnt > 0)
	{
		mCurrentUserCnt--;
	}
}

ERROR_CODE UserManager::AddUser(char* userID_, int clientIndex_)
{
	auto user_idx = clientIndex_;

	mUserObjPool[user_idx]->SetLogin(userID_);
	mUserIDDictionary.insert(std::pair< char*, int>(userID_, clientIndex_));

	return ERROR_CODE::NONE;
}

INT32 UserManager::FindUserIndexByID(char* userID_)
{
	auto res = mUserIDDictionary.find(userID_);
	if (res != mUserIDDictionary.end())
	{
		return (*res).second;
	}

	return -1;
}

void UserManager::DeleteUserInfo(User* user_)
{
	mUserIDDictionary.erase(user_->GetUserId());
	user_->Clear();
}