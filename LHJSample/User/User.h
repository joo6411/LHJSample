#pragma once
#include <string>
#include <map>
#include <unordered_map>
#include <functional>
#include "../Packet.h"
#include "State/UserState.h"

class ClientInfo;
class UserManager;
class RoomManager;
class Room;
enum class RESULT_CODE : unsigned short;

class User
{
	const UINT32 PACKET_DATA_BUFFER_SIZE = 8096;

public:
	User() = default;
	~User() = default;

	void Init(const INT32 index, UserManager* userManager, RoomManager* roomManager);
	void Clear();
	int SetLogin(std::string& userID_);
	RESULT_CODE EnterRoom(INT32 roomIndex_);
	RESULT_CODE LeaveRoom();
	INT32 GetCurrentRoomIndex() { return mRoomIndex; }
	INT32 GetNetConnIdx() { return mIndex; }

	std::string GetUserId() const { return  mUserID; }
	void SetPacketData(const UINT32 dataSize_, char* pData_);
	PacketInfo GetPacket();
	void ChangeState(UserState newState);
	UserState GetState() { return mState; }
	void ResetRecvFunction();
	bool CreateAccount(std::string& userID, std::string& password);
	RESULT_CODE CheckLoginable(UINT32 clientIndex_, std::string&  userID, std::string& password);
	std::vector<int> GetRoomList();
	Room* GetRoom();

	std::unordered_map<int, std::function<void(UINT16, char*)>> recvFuntionDictionary;
	std::function<void(UINT32, UINT32, char*)> sendPacketFunc;

private:
	INT32 mIndex = -1;
	INT32 mRoomIndex = -1;

	std::string mUserID;
	bool mIsConfirm = false;
	std::string mAuthToken;

	UserState mState;
	std::map<UserState, UserStateBase*> mStates;

	UINT32 mPacketDataBufferWPos = 0;
	UINT32 mPacketDataBufferRPos = 0;
	char* mPacketDataBuffer = nullptr;

	UserManager* mUserManager;
	RoomManager* mRoomManager;
};