#include "BasicEnemyController.hpp"
#include "State.hpp"
#include "PlayerController.hpp"

/////////////////////////////////////////////////////////////////////////////////////////////////
// Idle State

class BE_IdleState : public State 
{
public: 
    BasicEnemyController* unit;

    BE_IdleState(BasicEnemyController* unit)
    {
        this->unit = unit;
    }

    virtual ~BE_IdleState() = default;

    void Execute() override
    {
        if(!unit->aiStats.chasing && unit->aiStats.wanderTime <= 0)
        {
            PlayerController* player = BoardController::Get()->player;
            // Could move distanceToPlayer to blackboard and add timeSinceLastPlayerPositionCheck
            LogCustom(0, "Wandering...", nullptr);
            State* newState = unit->Wander(); 
            LogCustom(0, "Finished wandering", nullptr);
            if(newState != unit->currentState)
            {
                unit->ChangeState(newState);
            }
            unit->aiStats.wanderTime = unit->aiStats.wanderCooldown;
        }
        else
        {
            unit->aiStats.wanderTime -= GetFrameTime();
        }
    }

    void Entry(State* oldState) override
    {

    }

    void Exit(State* newState) override
    {

    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////
// Moving State

class BE_MovingState : public State 
{
public:
    BasicEnemyController* unit;
    float moveTime;
    bool stoppedMoving;
    float movementSnapThreshold;
    float worldMoveStep;
    Tile* targetTile;
    Vector3 direction;

    BE_MovingState(BasicEnemyController* unit, float movementSnapThreshold = 0.5f)
    {
        this->unit = unit;
        moveTime = 0;
        stoppedMoving = false;
        this->movementSnapThreshold = movementSnapThreshold;
        worldMoveStep = BoardController::Get()->GetWorldTileSpacing();
        targetTile = nullptr;
    }

    virtual ~BE_MovingState() = default;

    void Execute() override
    {
        if (moveTime >= movementSnapThreshold / unit->unitStats.movementSpeed)
        {
            unit->MoveToTile(targetTile, direction); 
        } else if (!stoppedMoving)
        {
            stoppedMoving = true;
            unit->occupiedTile->ClearTileObject();
            unit->SetOccupiedTile(targetTile);
            unit->transform.translation = targetTile->transform.translation;
        }

        if(moveTime <= 0)
        {
            unit->ChangeState(unit->idleState);
        }
        
        moveTime -= GetFrameTime();
    }
    void Entry(State* oldState) override
    {
        LogCustom(0, "Entering Moving State", nullptr);
        targetTile = static_cast<Tile*>(unit->blackboard.Get("targetTile"));
        stoppedMoving = false;
        moveTime = 1.0f / unit->unitStats.movementSpeed;
        direction = (targetTile->transform.translation - unit->transform.translation) 
        * (1 / movementSnapThreshold);
    }
    void Exit(State* newState) override
    {

    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////
// Chasing State

class BE_ChasingState : public State 
{
public:
    BasicEnemyController* unit;
    float chasingTime;

    BE_ChasingState(BasicEnemyController* unit, float movementSnapThreshold = 0.5f)
    {
        this->unit = unit;
    }

    virtual ~BE_ChasingState() = default;

    void Execute() override
    {
        PlayerController* player = BoardController::Get()->player;

        if(chasingTime <= 0)
        {
            if(player->unitStatus.attackable && BasicEnemyController::IsObjectWithinRange(unit, player, unit->unitStats.attackRange))
            {
                unit->ChangeState(unit->attackingState);
            } 
            else if(BasicEnemyController::IsObjectWithinRange(unit, player, unit->aiStats.chasingRange))
            {
                unit->Chase(player);
                chasingTime = unit->aiStats.chasingCooldown;    
            }
            else
            {
                unit->aiStats.chasing = false; 
                unit->ChangeState(unit->idleState);
            }
        }
        else
        {
            chasingTime -= GetFrameTime();
        }
    }

    void Entry(State* oldState) override
    {
        LogCustom(0, "Entered chasingState", nullptr);
    }
    void Exit(State* newState) override
    {

    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////
// Basic Enemy Controller

BasicEnemyController::BasicEnemyController()
{
    possibleMoves.push_back({0,1});
    possibleMoves.push_back({1,0});
    possibleMoves.push_back({0,-1});
    possibleMoves.push_back({-1,0});
    stoppedMoving = false;
    lastPlayerPosition = { 0 };

}

void BasicEnemyController::Start(GameObject* scene, GameState* gameState)
{
    UnitController::Start(scene, gameState);
    if(!started)
    {
        return;
    }
    tag = "Enemy";
    aiStats.wanderTime = aiStats.wanderCooldown + GetRandomValue(0, 1);
    idleState = new BE_IdleState(this);
    movingState = new BE_MovingState(this);
    chasingState = new BE_ChasingState(this);
    ChangeState(idleState);
}

void BasicEnemyController::Update(GameObject* scene, GameState* gameState)
{
    _CRT_UNUSED(scene);
    _CRT_UNUSED(gameState);
    
    currentState->Execute();
}

bool BasicEnemyController::IsObjectWithinRange(TileObject* object1, TileObject* object2, int range)
{
    int distance = GridPosition::Distance(object1->GetOccupiedTile()->gridPosition, object2->GetOccupiedTile()->gridPosition);

    // if(object == nullptr)
    // {
    //     LogCustom(0, "bad object", nullptr);
    //     return false;
    // }
    // else
    // {
    //     LogCustom(0, "First check successful", nullptr);
    //     printf("dafuq is going on");
    // }
    // printf("dafuq is going on");

    // LogCustom(0, "before getting distance", nullptr);
    // int distance = GridPosition::Distance(position, object->GetOccupiedTile()->gridPosition);
    // LogCustom(0, "after getting distance", nullptr);

    return distance <= range;
}

std::vector<GridPosition> BasicEnemyController::FindPathToObject(TileObject* object)
{
    LogCustom(0, "Pathfinding not yet implemented!", nullptr);
    return {};
}

void BasicEnemyController::OnTargetObjectKilled(IAttackable* target)
{
    UnitController::OnTargetObjectKilled(target);
    aiStats.chasing = false;
    ChangeState(idleState);
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Actions

// BasicEnemy Wander Action
// Returns next state:
// chasingState - Player is within agro range
// movingState - There is empty tile to wander to
// currentState - There are no possile actions
State* BasicEnemyController::Wander()
{
    // Transition to chasing
    PlayerController* player = BoardController::Get()->player;

    if(player == nullptr)
    {
        LogCustom(0, "PLAYER IS NULL DAFUQ", nullptr);
    }
    printf("Before IsObjectWithinRange");
    if(player->unitStatus.attackable && BasicEnemyController::IsObjectWithinRange(this, player, aiStats.agroRange))
    {
        // LLO here. Something doesn't make sense at all. Maybe scope problems?
    
        LogCustom(0, "test", nullptr);
        blackboard.Insert("targetTile", player->GetOccupiedTile());
        LogCustom(0, "test", nullptr);
        return chasingState;
    }

    LogCustom(0, "gonna return chasingState", nullptr);

    int randomDirection = GetRandomValue(0, 4);

    for(int i = 0; i < 4; i++)
    {
            targetTile = BoardController::Get()->GetTile(
            position + possibleMoves[(randomDirection + i) % 4]);
        
        if(targetTile != nullptr && !targetTile->IsStaticTile())
        {
            blackboard.Insert("targetTile", targetTile);
            TileObject* occupiedTileObject = targetTile->GetOccupiedTileObject();
            if(occupiedTileObject != nullptr)
            {
                // TODO: Enemy encountered smth
            } 
            else if (targetTile->SetTileObject(this))
            {
                // Can Move to tile
                return movingState;
            }
        }
    }

    return currentState;
}

// BasicEnemy Chase Action
// Returns next state:
// chasingState - Player is not within attacking range
// attackingState - Player is within attacking range 
State* BasicEnemyController::Chase(TileObject* object)
{
    std::vector<GridPosition> path = FindPathToObject(object);
    
    if(path.size() > 1 && BoardController::Get()->GetTile(path[1])->SetTileObject(this))
    {
        return movingState;
    }
    return chasingState;
}



