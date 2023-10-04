#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <list>
#include <vector>
#include <string>

// Ŭ���̾�Ʈ�� ���� ��Ŷ�� �����ϴ� ����ü
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
	REQ_CREATE_ACCOUNT = 200,
	ACK_CREATE_ACCOUNT = 201,

	REQ_LOGIN = 210,
	ACK_LOGIN = 211,

	REQ_LOBBY_INFO = 300,
	ACK_LOBBY_INFO = 301,
	NOTIFY_LOBBY_INFO = 302,

	REQ_ROOM_ENTER = 400,
	ACK_ROOM_ENTER = 401,

	REQ_ROOM_LEAVE = 410,
	ACK_ROOM_LEAVE = 411,
	NOTIFY_ROOM_LEAVE = 412,

	REQ_ROOM_CHAT = 420,
	ACK_ROOM_CHAT = 421,
	NOTIFY_ROOM_CHAT = 422,

	REQ_ROOM_INFO = 430,
	ACK_ROOM_INFO = 431,
	NOTIFY_ROOM_INFO = 432
};


#pragma pack(push,1)
struct PACKET_HEADER
{
	UINT16 PacketLength;
	UINT16 PacketId;
	UINT8 Type; //���࿩�� ��ȣȭ���� �� �Ӽ��� �˾Ƴ��� ��
};

const UINT32 PACKET_HEADER_LENGTH = sizeof(PACKET_HEADER);

//- ���� ���� ��û
const int MAX_USER_ID_LEN = 10;
const int MAX_USER_PW_LEN = 10;

struct REQ_CREATE_ACCOUNT_PACKET : public PACKET_HEADER
{
	char UserID[MAX_USER_ID_LEN + 1];
	char UserPW[MAX_USER_PW_LEN + 1];
};
const size_t REQ_CREATE_ACCOUNT_PACKET_SIZE = sizeof(REQ_CREATE_ACCOUNT_PACKET);

struct ACK_CREATE_ACCOUNT_PACKET : public PACKET_HEADER
{
	UINT16 Result;
};

//- �α��� ��û
struct REQ_LOGIN_PACKET : public PACKET_HEADER
{
	char UserID[MAX_USER_ID_LEN + 1];
	char UserPW[MAX_USER_PW_LEN + 1];
};
const size_t REQ_LOGIN_PACKET_SIZE = sizeof(REQ_LOGIN_PACKET);


struct ACK_LOGIN_PACKET : public PACKET_HEADER
{
	UINT16 Result;
};

struct REQ_LOBBY_INFO : public PACKET_HEADER
{
};

struct ACK_LOBBY_INFO : public PACKET_HEADER
{
	UINT16 Result;
};

struct NOTIFY_LOBBY_INFO : public PACKET_HEADER
{
	int RoomCount;
	std::vector<int> Room;
};

//- �뿡 ���� ��û
//const int MAX_ROOM_TITLE_SIZE = 32;
struct REQ_ROOM_ENTER_PACKET : public PACKET_HEADER
{
	INT32 RoomNumber;
};

struct ACK_ROOM_ENTER_PACKET : public PACKET_HEADER
{
	INT16 Result;
	//char RivalUserID[MAX_USER_ID_LEN + 1] = { 0, };
};


//- �� ���� ��û
struct REQ_ROOM_INFO_PACKET : public PACKET_HEADER
{
};

struct ACK_ROOM_INFO_PACKET : public PACKET_HEADER
{
	INT16 Result;
	int UserCount;
	std::vector<std::string> Users;
};

struct NOTIFY_ROOM_INFO_PACKET : public PACKET_HEADER
{
	char EnterUser[MAX_USER_ID_LEN + 1];
};

//- �� ������ ��û
struct REQ_ROOM_LEAVE_PACKET : public PACKET_HEADER
{
};

struct ACK_ROOM_LEAVE_PACKET : public PACKET_HEADER
{
	INT16 Result;
};

struct NOTIFY_ROOM_LEAVE_PACKET : public PACKET_HEADER
{
	char LeaveUser[MAX_USER_ID_LEN + 1];
};



// �� ä��
const int MAX_CHAT_MSG_SIZE = 256;
struct REQ_ROOM_CHAT_PACKET : public PACKET_HEADER
{
	char Message[MAX_CHAT_MSG_SIZE + 1] = { 0, };
};

struct ACK_ROOM_CHAT_PACKET : public PACKET_HEADER
{
	INT16 Result;
};

struct NOTIFY_ROOM_CHAT_PACKET : public PACKET_HEADER
{
	char UserID[MAX_USER_ID_LEN + 1] = { 0, };
	char Msg[MAX_CHAT_MSG_SIZE + 1] = { 0, };
};

#pragma pack(pop) //���� ������ ��ŷ������ �����