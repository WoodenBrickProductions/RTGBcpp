#ifndef UNITCONTROLLER_HPP
#define UNITCONTROLLER_HPP

#include "TileObject.hpp"
#include "UnitStatus.hpp"
#include "UnitStats.hpp"
#include "WorldStats.hpp"
#include "Interfaces/IAttackable.hpp"
#include "Interfaces/IDealsDamage.hpp"

class State;
class UnitController : public TileObject, public IAttackable, public IDealsDamage
{
public:
    UnitStatus unitStatus;
    UnitStats unitStats;

    UnitController();
    virtual ~UnitController() = default;
    void Start() override;

    // IAttackable
    bool GetAttacked(DamageSource damageSource) override;
    virtual void OnDeath(DamageSource damageSource) override;
    virtual GameObject* GetGameObject() override;

    // IDealsDamage
    bool Attack(IAttackable* target) override;
    void OnTargetObjectKilled(IAttackable* target) override;

    UnitStatus GetUnitStatus()
    {   
        return unitStatus;
    }

    UnitStats GetUnitStats()
    {
        return unitStats;
    }

protected:
    float moveTime;
    float attackTime;
    float attackCooldown;
    State* currentState;
    // float attackCooldown;
    float worldMoveStep;
    float worldScalling;
    Tile* targetTile;
    // UIController uiController;
    // UnitIndicatorController indicatorController;

    State* pausedState;
    State* idleState;
    State* movingState;
    State* attackingState;
    State* disabledState;
    State* chasingState;
    State* talkingState;
    State* searchingState;
    State* waitingState; 

    void MoveToTile(Tile* tile);
    void SetHealth(int healthChange);
    virtual void OnHealthReachesZero();
    virtual void ChangeState(State* newState);

};

#endif