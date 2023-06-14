#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// 클라이언트가 보낸 패킷을 저장하는 구조체
struct RawPacketData
{
	UINT32 ClientIndex = 0;
	UINT32 DataSize = 0;
	char* pPacketData = nullptr;

	void Set(RawPacketData& vlaue)
	{
		ClientIndex = vlaue.ClientIndex;
		DataSize = vlaue.DataSize;

		pPacketData = new char[vlaue.DataSize];
		CopyMemory(pPacketData, vlaue.pPacketData, vlaue.DataSize);
	}

	void Set(UINT32 clientIndex_, UINT32 dataSize_, char* pData)
	{
		ClientIndex = clientIndex_;
		DataSize = dataSize_;

		pPacketData = new char[dataSize_];
		CopyMemory(pPacketData, pData, dataSize_);
	}

	void Release()
	{
		delete pPacketData;
	}
};

struct PacketInfo
{
	UINT32 ClientIndex = 0;
	UINT16 PacketId = 0;
	UINT16 DataSize = 0;
	char* pDataPtr = nullptr;
};

enum class  PACKET_ID : UINT16
{
	//SYSTEM
	SYS_USER_CONNECT = 11,
	SYS_USER_DISCONNECT = 12,
	SYS_END = 30,

	//DB
	DB_END = 199,

	//Client
	LOGIN_REQUEST = 201,
	LOGIN_RESPONSE = 202,
};


#pragma pack(push,1)
struct PACKET_HEADER
{
	UINT16 PacketLength;
	UINT16 PacketId;
	UINT8 Type; //압축여부 암호화여부 등 속성을 알아내는 값
};

const UINT32 PACKET_HEADER_LENGTH = sizeof(PACKET_HEADER);

//- 로그인 요청
const int MAX_USER_ID_LEN = 32;
const int MAX_USER_PW_LEN = 32;

struct LOGIN_REQUEST_PACKET : public PACKET_HEADER
{
	char UserID[MAX_USER_ID_LEN + 1];
	char UserPW[MAX_USER_PW_LEN + 1];
};
const size_t LOGIN_REQUEST_PACKET_SZIE = sizeof(LOGIN_REQUEST_PACKET);


struct LOGIN_RESPONSE_PACKET : public PACKET_HEADER
{
	UINT16 Result;
};

#pragma once

enum class ERROR_CODE : unsigned short
{
	NONE = 0,

	USER_MGR_INVALID_USER_INDEX = 11,
	USER_MGR_INVALID_USER_UNIQUEID = 12,

	LOGIN_USER_ALREADY = 31,
	LOGIN_USER_USED_ALL_OBJ = 32,
	LOGIN_USER_INVALID_PW = 33,

	NEW_ROOM_USED_ALL_OBJ = 41,
	NEW_ROOM_FAIL_ENTER = 42,

	ENTER_ROOM_NOT_FINDE_USER = 51,
	ENTER_ROOM_INVALID_USER_STATUS = 52,
	ENTER_ROOM_NOT_USED_STATUS = 53,
	ENTER_ROOM_FULL_USER = 54,

	ROOM_INVALID_INDEX = 61,
	ROOM_NOT_USED = 62,
	ROOM_TOO_MANY_PACKET = 63,

	LEAVE_ROOM_INVALID_ROOM_INDEX = 71,

	CHAT_ROOM_INVALID_ROOM_NUMBER = 81,
};
