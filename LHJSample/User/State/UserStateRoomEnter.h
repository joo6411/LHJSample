#pragma once

#include "UserState.h"

class User;

class UserStateRoomEnter : public UserStateBase
{
public:
    virtual void Enter();
    virtual void Execute();
    virtual void Exit();

public:
    UserStateRoomEnter(User* user);
    virtual ~UserStateRoomEnter();
};