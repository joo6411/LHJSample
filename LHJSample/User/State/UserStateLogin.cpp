#include "UserStateLogin.h"
#include "../User.h"
#include "../../ResultCode.h"
#include <iostream>

UserStateLogin::UserStateLogin(User* user) :
    UserStateBase(user)
{

}

UserStateLogin::~UserStateLogin()
{
}

void UserStateLogin::Enter()
{
	mUser->ResetRecvFunction();
	mUser->recvFuntionDictionary[(int)PACKET_ID::ACK_CREATE_ACCOUNT] = [this](UINT16 packetSize, char* packet) -> void { ProcessCreateAccount(mUser->GetNetConnIdx(), packetSize, packet); };
	mUser->recvFuntionDictionary[(int)PACKET_ID::ACK_LOGIN] = [this](UINT16 packetSize, char* packet) -> void { ProcessLogin(mUser->GetNetConnIdx(), packetSize, packet); };
}

void UserStateLogin::Execute()
{
}

void UserStateLogin::Exit()
{

}