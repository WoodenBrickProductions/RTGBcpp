#include "PlayerController.hpp"
#include "CustomLogger.hpp"

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
}

void PlayerController::Start()
{
    if(occupiedTile == nullptr)
    {
        if(!boardController->ForceInitializePosition(*this))
        {
            LogCustom(2, "Couldn't place Player, no tile found", nullptr);
        }
    }
}

void PlayerController::Update()
{
    int movex = 0, movez = 0;
    if (IsKeyDown(KEY_A))
    {
        movex -= 1;
    }
    if (IsKeyDown(KEY_D))
    {
        movex += 1;
    }
    if (IsKeyDown(KEY_W))
    {
        movez -= 1;
    }
    if (IsKeyDown(KEY_S))
    {
        movez += 1;
    }

    float dtime = GetFrameTime();
    Vector3 tempMove = this->transform.translation;
    this->transform.translation = {tempMove.x + movex * dtime, 0.0f, tempMove.z + movez * dtime};
}