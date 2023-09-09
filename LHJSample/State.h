#pragma once

template <typename T>
class State
{
public:
    State(T owner) : Owner(owner) {};

public:
    virtual void Enter() = 0;
    virtual void Execute() = 0;
    virtual void Exit() = 0;
protected:
    T Owner;
};