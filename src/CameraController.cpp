#include "CameraController.hpp"
#include "CustomLogger.hpp"


CameraController* CameraController::main = nullptr;

CameraController::CameraController(CameraProjection cameraPerspective)
{
    GameObject* target = nullptr;
    camera = { 0 };
    camera.position = transform.translation;
    camera.target = { 0.0f, 0.0f, 0.0f };
    camera.up = { 0.0f, 1.0f, 0.0f };
    camera.fovy = 60.0f;
    camera.projection = cameraPerspective;
    cameraMovementSpeed = 1;
    followPosition = { 0 };
}
void CameraController::Start(GameObject* scene, GameState* gameState) 
{
    started = true;
    if(main == nullptr)
    {
        main = this;
        SetCameraMode(camera, CAMERA_CUSTOM);
    }

    if(target != nullptr)
    {
        cameraMovementSpeed = target->unitStats.movementSpeed;
        transform.translation = transform.translation + (target->transform.translation - followPosition); 
        followPosition = target->transform.translation;
        camera.target = followPosition;
        camera.position  = transform.translation;
    }

}
void CameraController::Update(GameObject* scene, GameState* gameState)
{
    // Log Camera Position
    // std::string output = std::to_string(transform.translation.x) + " " + std::to_string(transform.translation.y) + " " +std::to_string(transform.translation.z);
    // LogCustom(0, output.c_str(), nullptr);
    if(target->active && followPosition != target->transform.translation)
    {
        // LogCustom(0, "Camera is moving", nullptr);
        Vector3 vec1 = target->transform.translation - followPosition;
        Vector3 vec2 = vec1.Normalized() * GetFrameTime() * cameraMovementSpeed * BoardController::Get()->GetWorldTileSpacing();
        Vector3 vec3 = (vec1.Magnitude() <= vec2.Magnitude() ? vec1 : vec2);
        // LogCustom(0, std::to_string(vec1.Magnitude()).c_str(), nullptr);
        // LogCustom(0, std::to_string(vec2.Magnitude()).c_str(), nullptr);
        // LogCustom(0, std::to_string(vec3.Magnitude()).c_str(), nullptr);
        // LogCustom(0, std::to_string(vec3.x).c_str(), nullptr);
        // LogCustom(0, std::to_string(vec3.y).c_str(), nullptr);
        // LogCustom(0, std::to_string(vec3.z).c_str(), nullptr);

        transform.translation = transform.translation + vec3;
        followPosition = followPosition + vec3; 
    }

    camera.position = transform.translation;
    camera.target = followPosition;
}
