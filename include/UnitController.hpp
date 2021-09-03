#ifndef UNITCONTROLLER_HPP
#define UNITCONTROLLER_HPP

#include "TileObject.hpp"
#include "UnitStatus.hpp"
#include "UnitStats.hpp"
#include "WorldStats.hpp"
#include "IAttackable.hpp"
#include "IDealsDamage.hpp"
#include "State.hpp"

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
    virtual GameObject GetGameObject() override;

    // IDealsDamage
    bool Attack(IAttackable* target) override;
    void OnTargetObjectKilled(IAttackable target) override;

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
    int currentState;
    // float attackCooldown;
    float worldMoveStep;
    Tile* targetTile;
    // UIController uiController;
    // UnitIndicatorController indicatorController;

    void MoveToTile(Tile* tile);
    void SetHealth(int healthChange);
    virtual void OnHealthReachesZero();
    virtual void ChangeState(State* newState);

};

#endif