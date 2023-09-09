#pragma once
#include "UserState.h"
#include <windows.h>

class User;

class UserStateInLobby : public UserStateBase
{
public:
    virtual void Enter();
    virtual void Execute();
    virtual void Exit();

public:
    UserStateInLobby(User* user);
    virtual ~UserStateInLobby();

private:
    void ProcessLobbyInfo(UINT32 clientIndex_, UINT16 packetSize_, char* pPacket_);
    void ProcessEnterRoom(UINT32 clientIndex_, UINT16 packetSize_, char* pPacket_);
};