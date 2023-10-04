#pragma once
#include "../../State.h"

enum class UserState
{
	None,
    Connected,
    Disconnect,
    //Login,
    InLobby,
    RoomEnter,
    InRoom,
    RoomExit,
};

class User;

class UserStateBase
{
public:
    virtual void Enter() = 0;
    virtual void Execute() = 0;
    virtual void Exit() = 0;

public:
    UserStateBase(User* user) : mUser(user) {}
    virtual ~UserStateBase() {};

protected:
    User* mUser;
};