#ifndef IATTACKABLE_HPP
#define IATTACKABLE_HPP

class GameObject;
class DamageSource;
class IAttackable
{
public:
    virtual bool GetAttacked(DamageSource damageSource) = 0;
    virtual void OnDeath(DamageSource damageSource) = 0;
    virtual GameObject* GetGameObject() = 0;
};

#endif