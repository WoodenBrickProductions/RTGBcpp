#ifndef CAMERACONTROLLER_HPP
#define CAMERACONTROLLER_HPP

#include "UnitController.hpp"
#include "raylib.h"

class CameraController : public GameObject
{
public:
    CameraController(CameraProjection cameraPerspective);
    virtual ~CameraController() = default;
    
    void Start(GameObject* scene, GameState* gameState) override;
    void Update(GameObject* scene, GameState* gameState) override;

    static CameraController* main;
    
    UnitController* target;
    Vector3 followPosition;
    float cameraMovementSpeed;
    
    bool InsideViewFrustum(Vector3 centerPos, float radius);
    Camera GetCamera()
    {
        return camera;
    };


private:
    Camera camera;
};

#endif