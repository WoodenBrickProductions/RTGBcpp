#include "BasicEnemyController.hpp"
#include "State.hpp"
#include "PlayerController.hpp"
#include <queue>
#include <algorithm>

struct Node {
public:
    GridPosition position;
    std::vector<GridPosition> history;
};

/////////////////////////////////////////////////////////////////////////////////////////////////
// Idle State

class BE_IdleState : public State 
{
public: 
    BasicEnemyController* unit;

    BE_IdleState(BasicEnemyController* unit)
    {
        this->unit = unit;
        name = "IdleState";
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
        unit->aiStats.chasing = false;
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
        name = "MovingState";
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
            unit->ChangeState(unit->aiStats.chasing ? unit->chasingState : unit->idleState);
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
        name = "ChasingState";
    }

    virtual ~BE_ChasingState() = default;

    void Execute() override
    {
        PlayerController* player = BoardController::Get()->player;

        if(chasingTime <= 0)
        {
            if(player->unitStatus.attackable && BasicEnemyController::IsObjectWithinRange(unit, player, unit->unitStats.attackRange))
            {
                unit->blackboard.Insert("targetTile", player->GetOccupiedTile());
                unit->ChangeState(unit->attackingState);
            } 
            else if(BasicEnemyController::IsObjectWithinRange(unit, player, unit->aiStats.chasingRange))
            {
                State* newState = unit->Chase(player);
                if(newState != unit->currentState)
                {
                    unit->ChangeState(newState);
                }
                else
                {
                    chasingTime = unit->aiStats.chasingCooldown;    
                }
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
        unit->aiStats.chasing = true;
        LogCustom(0, "Entered chasingState", nullptr);
    }
    void Exit(State* newState) override
    {

    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////
// Attacking State

class BE_AttackingState : public State
{
public:
    BasicEnemyController* unit;
    Tile* targetTile;

    BE_AttackingState(BasicEnemyController* unit)
    {
        this->unit = unit;
        name = "AttackingState";
        targetTile = nullptr;
    }

    void Execute() override
    {
        if(unit->unitStats.attackTime <= 0)
        {
            LogCustom(0, "Attacking");
            TileObject* occupiedTileObject = targetTile->GetOccupiedTileObject();
            if(occupiedTileObject != nullptr && occupiedTileObject->tag == "Player")
            {
                PlayerController* targetUnit = (PlayerController*) occupiedTileObject;
                if(targetUnit->unitStatus.attackable)
                {
                    if(unit->Attack(targetUnit))
                    {
                        unit->unitStats.attackTime = 1.0f / unit->unitStats.attackSpeed;  
                    }
                    else
                    {
                        unit->aiStats.chasing = false;
                        unit->ChangeState(unit->idleState);
                    }
                }
                else
                {
                    unit->ChangeState(unit->idleState);
                }
            }
            else
            {
                unit->ChangeState(unit->chasingState);
            }
        }
        
    }

    void Entry(State* oldState) override
    {
        targetTile = static_cast<Tile*>(unit->blackboard.Get("targetTile"));
        unit->unitStats.attackTime = 1.0f / unit->unitStats.attackSpeed;  

        if(targetTile == nullptr)
        {
            unit->ChangeState(unit->idleState);
            return;
        }
    }

    virtual void Exit(State* newState) override
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
    attackingState = new BE_AttackingState(this);
    ChangeState(idleState);
}

void BasicEnemyController::Update(GameObject* scene, GameState* gameState)
{
    std::string out = "Current state: ";
    out.append(currentState->name);
    LogCustom(0, out.c_str());
    currentState->Execute();

    if(unitStats.attackTime >= 0)
    {
        unitStats.attackTime -= GetFrameTime();
    }
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
    Node start;
    start.position = occupiedTile->gridPosition;
    std::vector<GridPosition> result;
    std::vector<GridPosition> visited;
    std::queue<Node> work;

    visited.push_back(start.position);
    work.push(start);

    while(work.size() > 0)
    {
        Node current = work.front();
        work.pop();

        if((int) current.history.size() > aiStats.chasingRange)
        {
            LogCustom(0, "Player outside range");
            return {};
        }
        Tile* currentTile = boardController->GetTile(current.position);
        if(currentTile != nullptr)
        {
            TileObject* currentOccupiedObject = currentTile->GetOccupiedTileObject();
            if(currentOccupiedObject != nullptr && currentOccupiedObject->tag == "Player")
            {
                // Found Node
                result = current.history;
                result.push_back(current.position);
                std::string out = "Result size: ";
                out.append("" + result.size());
                out.append("\n");
                out.append(result.back().ToString() + "\n");
                LogCustom(0, out.c_str());
                return result;
            }

            for(unsigned int i = 0; i < possibleMoves.size(); i++)
            {
                Tile* neighborTile = boardController->GetTile(possibleMoves[i] + current.position);
                if(neighborTile != nullptr && !neighborTile->staticTile && 
                (neighborTile->GetOccupiedTileObject() == nullptr || neighborTile->GetOccupiedTileObject()->tag == "Player"))
                {
                    Node currentNeighbor;
                    currentNeighbor.position = neighborTile->gridPosition;
                    if(std::find(visited.begin(), visited.end(), currentNeighbor.position) == visited.end())
                    {
                        currentNeighbor.history = (current.history);
                        currentNeighbor.history.push_back(current.position);
                        visited.push_back(currentNeighbor.position);
                        work.push(currentNeighbor);
                    }
                }
            }
        }
    }

    LogCustom(0, "Pathfinding default return");
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
    
    if(path.size() == 0)
    {
        return idleState;
    }

    if((int) path.size() < unitStats.attackRange)
    {
        blackboard.Insert("targetTile", BoardController::Get()->GetTile(path.back()));
        return attackingState;
    }

    Tile* tile = BoardController::Get()->GetTile(path[1]);
    if(tile->SetTileObject(this))
    {
        blackboard.Insert("targetTile", tile);
        return movingState;
    }

    return chasingState;
}



