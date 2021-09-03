#ifndef IDEALSDAMAGE_HPP
#define IDEALSDAMAGE_HPP

class GameObject;
class IAttackable;
class IDealsDamage
{
public:
    virtual ~IDealsDamage() = 0;
    virtual bool Attack(IAttackable* target) = 0;
    virtual void OnTargetObjectKilled(IAttackable target) = 0;
};

#endif