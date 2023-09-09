#include "UserStateConnected.h"
#include "../User.h"
#include "../../ResultCode.h"
#include <iostream>

UserStateConnected::UserStateConnected(User* user) :
    UserStateBase(user)
{

}

UserStateConnected::~UserStateConnected()
{
}

void UserStateConnected::Enter()
{
	mUser->ResetRecvFunction();
	mUser->recvFuntionDictionary[(int)PACKET_ID::REQ_CREATE_ACCOUNT] = [this](UINT16 packetSize, char* packet) -> void { ProcessCreateAccount(mUser->GetNetConnIdx(), packetSize, packet); };
	mUser->recvFuntionDictionary[(int)PACKET_ID::REQ_LOGIN] = [this](UINT16 packetSize, char* packet) -> void { ProcessLogin(mUser->GetNetConnIdx(), packetSize, packet); };
}

void UserStateConnected::Execute()
{
}

void UserStateConnected::Exit()
{

}

void UserStateConnected::ProcessCreateAccount(UINT32 clientIndex_, UINT16 packetSize_, char* pPacket_)
{
	if (REQ_CREATE_ACCOUNT_PACKET_SIZE != packetSize_)
	{
		return;
	}

	auto packet = reinterpret_cast<REQ_CREATE_ACCOUNT_PACKET*>(pPacket_);
	RESULT_CODE err = RESULT_CODE::NONE;
	std::string userID(packet->UserID);
	std::string password(packet->UserPW);
	if (!mUser->CreateAccount(userID, password))
	{
		err = RESULT_CODE::CREATE_ACCOUNT_FAIL;
	}

	ACK_CREATE_ACCOUNT_PACKET createAccountPacket;
	createAccountPacket.PacketId = (UINT16)PACKET_ID::ACK_CREATE_ACCOUNT;
	createAccountPacket.Result = (UINT16)err;
	createAccountPacket.PacketLength = sizeof(ACK_CREATE_ACCOUNT_PACKET);

	mUser->sendPacketFunc(clientIndex_, sizeof(ACK_CREATE_ACCOUNT_PACKET), (char*)&createAccountPacket);
}

void UserStateConnected::ProcessLogin(UINT32 clientIndex_, UINT16 packetSize_, char* pPacket_)
{
	if (REQ_LOGIN_PACKET_SIZE != packetSize_)
	{
		return;
	}

	auto packet = reinterpret_cast<REQ_LOGIN_PACKET*>(pPacket_);
	
	std::string userID(packet->UserID);
	std::string password(packet->UserPW);
	std::cout << "requested user id = " << userID << std::endl;

	ACK_LOGIN_PACKET ackLoginPacket;
	ackLoginPacket.PacketId = (UINT16)PACKET_ID::ACK_LOGIN;
	ackLoginPacket.PacketLength = sizeof(ACK_LOGIN_PACKET);
	ackLoginPacket.Result= (UINT16)mUser->CheckLoginable(clientIndex_, userID, password);
	
	if (ackLoginPacket.Result == (UINT16)RESULT_CODE::LOGIN_SUCCESS)
	{
		mUser->SetLogin(userID);
	}
	else
	{
		std::cout << "LoginFail id = " << userID << ", resultCode = " << (int)ackLoginPacket.Result << std::endl;
	}

	mUser->sendPacketFunc(clientIndex_, sizeof(ACK_LOGIN_PACKET), (char*)&ackLoginPacket);
}