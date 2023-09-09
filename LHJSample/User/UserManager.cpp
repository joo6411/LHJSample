#include "UserManager.h"
#include "../DB/AccountDB.h"

void UserManager::Init(const INT32 maxUserCount_, AccountDB* accountDB, RoomManager* roomManager)
{
	mMaxUserCnt = maxUserCount_;
	mUserObjPool = std::vector<User*>(mMaxUserCnt);
	mAccountDB = accountDB;

	for (auto i = 0; i < mMaxUserCnt; i++)
	{
		mUserObjPool[i] = new User();
		mUserObjPool[i]->Init(i, this, roomManager);
	}
}

void UserManager::SetSendPacketFunc(std::function<void(UINT32, UINT32, char*)> sendpacketfunc)
{
	for (auto i = 0; i < mMaxUserCnt; i++)
	{
		mUserObjPool[i]->sendPacketFunc = sendpacketfunc;
	}
}

void UserManager::DecreaseUserCnt()
{
	if (mCurrentUserCnt > 0)
	{
		mCurrentUserCnt--;
	}
}

RESULT_CODE UserManager::AddUser(std::string& userID_, int clientIndex_)
{
	auto user_idx = clientIndex_;

	mUserObjPool[user_idx]->SetLogin(userID_);
	mUserIDDictionary.insert(std::pair<std::string, int>(userID_, clientIndex_));

	return RESULT_CODE::NONE;
}

INT32 UserManager::FindUserIndexByID(std::string& userID_)
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

RESULT_CODE UserManager::CheckLoginable(UINT32 clientIndex_, std::string& userID, std::string& password)
{
	RESULT_CODE result = RESULT_CODE::NONE;
	if (GetCurrentUserCnt() >= GetMaxUserCnt())
	{
		//접속자수가 최대수를 차지해서 접속불가
		result = RESULT_CODE::LOGIN_USER_USED_ALL_OBJ;
		return result;
	}

	if (FindUserIndexByID(userID) == -1)
	{
		/*
		RedisLoginReq dbReq;
		CopyMemory(dbReq.UserID, pLoginReqPacket->UserID, (MAX_USER_ID_LEN + 1));
		CopyMemory(dbReq.UserPW, pLoginReqPacket->UserPW, (MAX_USER_PW_LEN + 1));

		RedisTask task;
		task.UserIndex = clientIndex_;
		task.TaskID = RedisTaskID::REQUEST_LOGIN;
		task.DataSize = sizeof(RedisLoginReq);
		task.pData = new char[task.DataSize];
		CopyMemory(task.pData, (char*)&dbReq, task.DataSize);
		mRedisMgr->PushTask(task);

		std::cout<<("Login To Redis user id = %s\n", pUserID);
		*/
		if (mAccountDB->SelectAccount(userID, password))
		{
			AddUser(userID, clientIndex_);
			result = RESULT_CODE::LOGIN_SUCCESS;
		}
		else
		{
			result = RESULT_CODE::LOGIN_USER_INVALID_PW;
		}
	}
	else
	{
		//접속중인 유저여서 실패를 반환한다.
		result = RESULT_CODE::LOGIN_USER_ALREADY;
	}

	return result;
}

bool UserManager::CreateAccount(std::string& userID, std::string& password)
{
	return mAccountDB->CreateAccount(userID, password);
}