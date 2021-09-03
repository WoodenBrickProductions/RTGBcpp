#ifndef STATE_HPP
#define STATE_HPP

class State
{
public:
    virtual ~State() = default;
    virtual void execute();
    virtual void entry(State oldState);
    virtual void exit(State newState);
};
#endif