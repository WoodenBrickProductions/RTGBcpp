#ifndef STATE_HPP
#define STATE_HPP

class State
{
public:
    virtual ~State() = default;
    virtual void Execute();
    virtual void Entry(State* oldState);
    virtual void Exit(State* newState);
};
#endif