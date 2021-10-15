#include "PlayerController.hpp"
#include "CustomLogger.hpp"
#include "State.hpp"

class Player_IdleState : public State
{
public:
    PlayerController* player;
    Direction input;
    Blackboard* blackboard;

    Player_IdleState(PlayerController* player)
    {
        this->player = player;
        blackboard = &player->blackboard;
    }

    // State'ai valdo playerį, o ne playeris state'us. More modular approach
    void Execute()
    {
        LogCustom(0, "Executing in IdleState", nullptr);

        input = player->GetInputDirection();
        if(input != None)
        {
            GridPosition targetPosition = player->GetTargetPosition(input);
            LogCustom(0, "Getting input", nullptr);
            // Tile* targetTile = static_cast<Tile*>(malloc(sizeof(Tile)));
            Tile* targetTile = BoardController::Get()->GetTile(targetPosition);
            

            blackboard->Insert("targetTile", targetTile);
            Tile* newTile = (Tile*) blackboard->Get("targetTile");
            std::string out = "Blackboard value: ";

            out.append(newTile == targetTile ? "yes" : "no");
            printf("%p", targetTile);
            printf("\n");
            printf("%p", (void*) targetTile);
            printf("\n");
            printf("%p", newTile);
            LogCustom(0, out.c_str(), nullptr);
            if (targetTile != nullptr && !targetTile->IsStaticTile())
            {
                TileObject* occupiedTileObject = targetTile->GetOccupiedTileObject();
                if(occupiedTileObject != nullptr && !occupiedTileObject->staticObject)
                {
                    // Implementation for changing to attackState;
                }
                else
                {
                    if(targetTile->SetTileObject(player))
                    {
                        player->ChangeState(player->movingState); // This feels wrong, don't know why
                    }
                }
            }   
        }
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

class Player_MovingState : public State
{
    public: 
    PlayerController* player;
    Blackboard* blackboard;
    float moveTime;
    bool stoppedMoving;
    float movementSnapThreshold;
    Vector3 direction;
    float worldMoveStep;
    Tile* targetTile;


    Player_MovingState(PlayerController* player, float movementSnapThreshold = 0.5f)
    {
        this->player = player;
        blackboard = &player->blackboard;
        moveTime = 0;
        stoppedMoving = false;
        this->movementSnapThreshold = movementSnapThreshold;
        worldMoveStep = BoardController::Get()->GetWorldTileSpacing();
        targetTile = nullptr;
    };

    void Execute()
    {

        if (moveTime >= movementSnapThreshold / player->unitStats.movementSpeed)
        {
            player->MoveToTile(targetTile, direction); // Last left off here, made a blackboard, now I'm playing around with static casts
            // Last left off here, smth wrong here
        } else if (!stoppedMoving)
        {
            stoppedMoving = true;
            player->occupiedTile->ClearTileObject();
            player->SetOccupiedTile(targetTile);
            player->transform.translation = targetTile->transform.translation;
        }

        if(moveTime <= 0)
        {
            player->ChangeState(player->idleState);
        }
        
        moveTime -= GetFrameTime();
    }

    void Entry(State* oldState)
    {
        targetTile = static_cast<Tile*>(blackboard->Get("targetTile"));
        LogCustom(0, "Entering MovingState", nullptr);
        stoppedMoving = false;
        moveTime = 1.0f / player->unitStats.movementSpeed;
        direction = (targetTile->transform.translation - player->transform.translation) * (1 / movementSnapThreshold);
    }

    void Exit(State* newState)
    {
        LogCustom(0, "Exiting MovingState", nullptr);
    }
};

PlayerController::PlayerController()
{
    // TileObject
    name = typeid(this).name();
    tag = "Player";
    staticObject = false;

    // PlayerController
    moveStepMultipier = 1;
    stoppedMoving = true;
    inputDirection = None;
    newInput = None;
    changeInput = None;
    blackboard.Insert("targetTile", new Tile());

    // UnitStats
    unitStats.movementSpeed = 5;
}

void PlayerController::Start(GameObject* scene, GameState* state)
{
    UnitController::Start(scene, state);
    idleState = new Player_IdleState(this);
    movingState = new Player_MovingState(this);

    ChangeState(idleState);
}

void PlayerController::OnFailedToInitialize()
{
    if(!boardController->ForceInitializePosition(*this))
    {
        LogCustom(2, "Couldn't place Player, no tile found", nullptr);
    }
}

void PlayerController::Update(GameObject* scene, GameState* state)
{
    _CRT_UNUSED(scene);
    _CRT_UNUSED(state);

    currentState->Execute();
    // int movex = 0, movez = 0;
    // if (IsKeyDown(KEY_A))
    // {
    //     movex -= 1;
    // }
    // if (IsKeyDown(KEY_D))
    // {
    //     movex += 1;
    // }
    // if (IsKeyDown(KEY_W))
    // {
    //     movez -= 1;
    // }
    // if (IsKeyDown(KEY_S))
    // {
    //     movez += 1;
    // }

    // float dtime = GetFrameTime();
    // Vector3 tempMove = this->transform.translation;
    // this->transform.translation = {tempMove.x + movex * dtime, 0.0f, tempMove.z + movez * dtime};
}

Direction PlayerController::GetInputDirection()
{
    bool up = IsKeyDown(KEY_W);
    bool right = IsKeyDown(KEY_D);
    bool down = IsKeyDown(KEY_S);
    bool left = IsKeyDown(KEY_A);
    
    if (up && !(right || down || left))
    {
        return Up;
    }
    else if (right && !(up || down || left))
    {
        return Right;
    }
    else if (down && !(right || up || left))
    {
        return Down;
    }
    else if (left && !(right || down || up))
    {
        return Left;
    }

    return None;
}

GridPosition PlayerController::GetTargetPosition(Direction direction)
{
    switch(direction)
    {
        case Up:
            return GridPosition{position.x, position.y - 1};
            break;
        case Right:
            return GridPosition{position.x + 1, position.y};
            break;
        case Down:
            return GridPosition{position.x, position.y + 1};
            break;
        case Left:
            return GridPosition{position.x - 1, position.y};
            break;
        default:    
            return GridPosition();
            break;
    }
}
