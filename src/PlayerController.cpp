#include "PlayerController.hpp"
#include "stdio.h"

PlayerController::PlayerController()
{
}

void PlayerController::SetTempLogger(TraceLogCallback callback)
{
    this->callback = callback;
}

void PlayerController::Update()
{
    callback(0, "This is running in Player Update", nullptr);
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