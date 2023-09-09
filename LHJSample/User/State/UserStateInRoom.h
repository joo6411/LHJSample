#pragma once

#include "UserState.h"
#include <windows.h>

class User;

class UserStateInRoom : public UserStateBase
{
public:
    virtual void Enter();
    virtual void Execute();
    virtual void Exit();

public:
    UserStateInRoom(User* user);
    virtual ~UserStateInRoom();

private:
    void ProcessRoomInfo(UINT32 clientIndex_, UINT16 packetSize_, char* pPacket_);
    void ProcessLeaveRoom(UINT32 clientIndex_, UINT16 packetSize_, char* pPacket_);
    void ProcessRoomChatMessage(UINT32 clientIndex_, UINT16 packetSize_, char* pPacket_);
};