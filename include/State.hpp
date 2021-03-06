#ifndef STATE_HPP
#define STATE_HPP

class State
{
public:
    std::string name = "State";
    virtual ~State() = default;
    virtual void Execute() = 0;
    virtual void Entry(State* oldState) = 0;
    virtual void Exit(State* newState) = 0;
};
#endif