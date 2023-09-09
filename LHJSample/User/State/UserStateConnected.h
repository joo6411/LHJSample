#pragma once

#include "UserState.h"
#include <windows.h>

class User;

class UserStateConnected : public UserStateBase
{
public:
    virtual void Enter();
    virtual void Execute();
    virtual void Exit();

public:
    UserStateConnected(User* user);
    virtual ~UserStateConnected();

private:
    void ProcessCreateAccount(UINT32 clientIndex_, UINT16 packetSize_, char* pPacket_);
    void ProcessLogin(UINT32 clientIndex_, UINT16 packetSize_, char* pPacket_);
};