#pragma once

#include "UserState.h"

class User;

class UserStateNone : public UserStateBase
{
public:
	virtual void Enter();
    virtual void Execute();
    virtual void Exit();

public:
    UserStateNone(User* user);
    virtual ~UserStateNone();
};