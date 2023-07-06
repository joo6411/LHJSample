#include "RedisManager.h"
#include "../thirdparty/CRedisConn.h"

void RedisManager::Init()
{
	mConn = new RedisCpp::CRedisConn();
}

bool RedisManager::Run(std::string ip_, UINT16 port_, const UINT32 threadCount_)
{
	if (Connect(ip_, port_) == false)
	{
		std::cout<<("Redis 접속 실패\n");
		return false;
	}

	mIsTaskRun = true;

	for (UINT32 i = 0; i < threadCount_; i++)
	{
		mTaskThreads.emplace_back([this]() { TaskProcessThread(); });
	}

	std::cout<<("Redis 동작 중...\n");
	return true;
}

void RedisManager::End()
{
	mIsTaskRun = false;

	for (auto& thread : mTaskThreads)
	{
		if (thread.joinable())
		{
			thread.join();
		}
	}

	delete mConn;
}

void RedisManager::PushTask(RedisTask task_)
{
	std::lock_guard<std::mutex> guard(mReqLock);
	mRequestTask.push_back(task_);
}

RedisTask RedisManager::TakeResponseTask()
{
	std::lock_guard<std::mutex> guard(mResLock);

	if (mResponseTask.empty())
	{
		return RedisTask();
	}

	auto task = mResponseTask.front();
	mResponseTask.pop_front();

	return task;
}


bool RedisManager::Connect(std::string ip_, UINT16 port_)
{
	if (mConn->connect(ip_, port_) == false)
	{
		std::cout << "connect error " << mConn->getErrorStr() << std::endl;
		return false;
	}
	else
	{
		std::cout << "connect success !!!" << std::endl;
	}

	return true;
}

void RedisManager::TaskProcessThread()
{
	std::cout<<("Redis 스레드 시작...\n");

	while (mIsTaskRun)
	{
		bool isIdle = true;
		auto task = TakeRequestTask();

		if (task.TaskID != RedisTaskID::INVALID)
		{
			isIdle = false;

			/*
			if (task.TaskID == RedisTaskID::REQUEST_LOGIN)
			{
				auto pRequest = (RedisLoginReq*)task.pData;

				RedisLoginRes bodyData;
				bodyData.Result = (UINT16)RESULT_CODE::LOGIN_USER_INVALID_PW;

				std::string value;
				if (mConn->get(pRequest->UserID, value))
				{
					bodyData.Result = (UINT16)RESULT_CODE::NONE;

					if (value.compare(pRequest->UserPW) == 0)
					{
						bodyData.Result = (UINT16)RESULT_CODE::NONE;
					}
				}
				else
				{
					std::cout << "User " << pRequest->UserID << " Created\n";
					//mConn->hset(pRequest->UserID, , pRequest->UserPW,);
					bodyData.Result = (UINT16)RESULT_CODE::NONE;
				}

				RedisTask resTask;
				resTask.UserIndex = task.UserIndex;
				resTask.TaskID = RedisTaskID::RESPONSE_LOGIN;
				resTask.DataSize = sizeof(RedisLoginRes);
				resTask.pData = new char[resTask.DataSize];
				CopyMemory(resTask.pData, (char*)&bodyData, resTask.DataSize);

				PushResponse(resTask);
			}
			*/
			task.Release();
		}


		if (isIdle)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}

	std::cout<<("Redis 스레드 종료\n");
}

RedisTask RedisManager::TakeRequestTask()
{
	std::lock_guard<std::mutex> guard(mReqLock);

	if (mRequestTask.empty())
	{
		return RedisTask();
	}

	auto task = mRequestTask.front();
	mRequestTask.pop_front();

	return task;
}

void RedisManager::PushResponse(RedisTask task_)
{
	std::lock_guard<std::mutex> guard(mResLock);
	mResponseTask.push_back(task_);
}
