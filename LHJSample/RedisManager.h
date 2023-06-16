#pragma once

#include "RedisTaskDefine.h"

#include <vector>
#include <deque>
#include <thread>
#include <mutex>

namespace RedisCpp
{
	class CRedisConn;
}

class RedisManager
{
public:
	RedisManager() = default;
	~RedisManager() = default;

	void Init();
	bool Run(std::string ip_, UINT16 port_, const UINT32 threadCount_);
	void End();
	void PushTask(RedisTask task_);
	RedisTask TakeResponseTask();

private:
	bool Connect(std::string ip_, UINT16 port_);
	void TaskProcessThread();
	RedisTask TakeRequestTask();
	void PushResponse(RedisTask task_);

	RedisCpp::CRedisConn* mConn;

	bool		mIsTaskRun = false;
	std::vector<std::thread> mTaskThreads;

	std::mutex mReqLock;
	std::deque<RedisTask> mRequestTask;

	std::mutex mResLock;
	std::deque<RedisTask> mResponseTask;
};