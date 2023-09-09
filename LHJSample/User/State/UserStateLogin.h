#pragma once

#include "UserState.h"

class User;

class UserStateLogin : public UserStateBase
{
public:
    virtual void Enter();
    virtual void Execute();
    virtual void Exit();

public:
    UserStateLogin(User* user);
    virtual ~UserStateLogin();
};