#include "UnitController.hpp"
#include "Data/DamageSource.hpp"
#include "Interfaces/IAttackable.hpp"
#include "State.hpp"
#include "CustomLogger.hpp"

class PausedState : public State
{
    void Execute()
    {
        LogCustom(0, "Executing in PausedState", nullptr);
    }

    void Entry(State* oldState)
    {
        LogCustom(0, "Entering PausedState", nullptr);
    }

    void Exit(State* newState)
    {
        LogCustom(0, "Exiting PausedState", nullptr);
    }
};

class IdleState : public State
{
    void Execute()
    {
        LogCustom(0, "Executing in IdleState", nullptr);
    }

    void Entry(State* oldState)
    {
        LogCustom(0, "Entering IdleState", nullptr);
    }

    void Exit(State* newState)
    {
        LogCustom(0, "Exiting IdleState", nullptr);
    }
};

class MovingState : public State
{
    void Execute()
    {
        LogCustom(0, "Executing in MovingState", nullptr);
    }

    void Entry(State* oldState)
    {
        LogCustom(0, "Entering MovingState", nullptr);
    }

    void Exit(State* newState)
    {
        LogCustom(0, "Exiting MovingState", nullptr);
    }
};

class DisabledState : public State
{
    void Execute()
    {

    }

    void Entry(State* oldState)
    {

    }

    void Exit(State* newState)
    {
        
    }
};

UnitController::UnitController()
{
    moveTime = 0;
    attackTime = 0;
    attackCooldown = 1;
    currentState = nullptr;
    worldMoveStep = 1;
    moveStepMultipier = 1;
    worldScalling = 1;
    Tile* targetTile = nullptr;
    // STATES ONLY RESERVED LOCATIONS
    // I want the states to be saved together with the object, but I don't want every object to have an instance of every state.
    pausedState = nullptr;
    idleState = nullptr;
    movingState = nullptr;
    attackingState = nullptr;
    disabledState = nullptr;
    chasingState = nullptr;
    talkingState = nullptr;
    searchingState = nullptr;
    waitingState = nullptr; 
}

void UnitController::Start(GameObject* scene, GameState* gameState) 
{
    TileObject::Start(scene, gameState);
    pausedState = new PausedState();
    idleState = new IdleState();
    movingState = new MovingState();
    currentState = pausedState;
    worldScalling = boardController->GetWorldTileSpacing();
    worldMoveStep = unitStats.movementSpeed * worldScalling;
    unitStats.currentHealth = unitStats.maxHealth;
    attackCooldown = 1.0f / unitStats.attackSpeed;
}

// IAttackable
bool UnitController::GetAttacked(DamageSource damageSource)
{
    if (unitStatus.attackable)
    {
        SetHealth(damageSource.damageAmount);
        if (unitStats.currentHealth == 0)
        {
            damageSource.source->OnTargetObjectKilled(this);
            OnDeath(damageSource);
        }
        return true;
    }
    return false;
}
void UnitController::OnDeath(DamageSource damageSource) 
{
    occupiedTile->ClearTileObject();
    LogCustom(0, "This unit has been killed", nullptr);
    // TODO: Update UI
    Destroy();
}
GameObject* UnitController::GetGameObject() 
{
    return this;
}

// IDealsDamage
bool UnitController::Attack(IAttackable* target) 
{
    return target->GetAttacked(DamageSource(this, DAMAGE_TYPE_ATTACK, -unitStats.attackDamage));
}

void UnitController::OnTargetObjectKilled(IAttackable* target) 
{
    std::string out = "I: ";
    out.append(name);
    out.append(" killed ");
    out.append(target->GetGameObject()->name);
    LogCustom(0, out.c_str(), nullptr); // LAST LEFT OFF
}

void UnitController::MoveToTile(Tile* tile, Vector3 direction)
{
    float dTime = GetFrameTime();
    // transform.translation = transform.translation + (tile->transform.translation - transform.translation) * dTime * moveStepMultiplier * worldMoveStep;
    Vector3 vec1 = direction * dTime * worldMoveStep;
    Vector3 vec2 = tile->transform.translation - transform.translation;
    transform.translation = transform.translation + (vec1.Magnitude() <= vec2.Magnitude() ? vec1 : vec2);  // Last left off here.
}

void UnitController::SetHealth(int healthChange)
{
    if (unitStats.currentHealth + healthChange <= 0)
    {
        unitStats.currentHealth = 0;
        OnHealthReachesZero();
    }
    else if (unitStats.currentHealth + healthChange > unitStats.currentHealth)
    {
        unitStats.currentHealth = unitStats.maxHealth;
    }
    else
    {
        unitStats.currentHealth += healthChange;
    }
    // uiController.SetHealth(this);
}

void UnitController::OnHealthReachesZero()
{
    unitStatus.attackable = false;
    ChangeState(disabledState);
}

void UnitController::ChangeState(State* newState)
{
    State* oldState = currentState;
    currentState = newState;
    oldState->Exit(newState);
    currentState->Entry(oldState);
}



