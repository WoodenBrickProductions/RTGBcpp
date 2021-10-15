#ifndef GAMECONTROLLER_HPP
#define GAMECONTROLLER_HPP

#include "raylib.h"
class GameObject;

struct Resources
{
    Model cubeTriangulated;
    Model smallCube;
    Model smallWall;
    Model pit;
    Texture2D placeholderTexture;
};

struct GameState
{
    float gameTime;
    bool gameStarted;
};

void Instantiate(GameObject* gameObject, GameObject* scene, GameState* gameState);
void Destroy(GameObject* gameObject);

#endif