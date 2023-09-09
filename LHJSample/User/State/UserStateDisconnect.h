#pragma once

#include "UserState.h"

class User;

class UserStateDisConnect : public UserStateBase
{
public:
    virtual void Enter();
    virtual void Execute();
    virtual void Exit();

public:
    UserStateDisConnect(User* user);
    virtual ~UserStateDisConnect();
};