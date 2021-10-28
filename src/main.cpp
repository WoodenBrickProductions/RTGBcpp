/*******************************************************************************************
*
*   raylib [core] example - Basic 3d example
*
*   Welcome to raylib!
*
*   To compile example, just press F5.
*   Note that compiled executable is placed in the same folder as .c file
*
*   You can find all basic examples on C:\raylib\raylib\examples folder or
*   raylib official webpage: www.raylib.com
*
*   Enjoy using raylib. :)
*
*   This example has been created using raylib 1.0 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2013-2020 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#define UNUSED(x) (void)(x)

#include "GameController.hpp"
#include "CameraController.hpp"
#include "BoardController.hpp"
#include "Tile.hpp"
#include "PlayerController.hpp"
#include "BasicEnemyController.hpp"
#include "UnitIndicator.hpp"
#include "CustomLogger.hpp"
#include <stdio.h>
#include <time.h>
#include <vector>
#include <string>


const auto LOG_CALLBACK = LogCustom;

const int SCREENWIDTH = 800;
const int SCREENHEIGHT = 450;
const int GRIDX = 100;
const int GRIDY = 100;

BoardController* gBoardController;
GameObject* gScene;
GameState* gGameState;
CameraController* gCameraController;
PlayerController* gPlayerController;
std::vector<GameObject*> gGameObjects;
std::vector<GameObject*> gGameObjectsToBeRemoved;

/////////////////////////////////////////////////////////////////////////////////////////////////
// Helper functions
/////////////////////////////////////////////////////////////////////////////////////////////////

std::string ToString(Vector3 vec)
{
    return std::to_string(vec.x) + " " + std::to_string(vec.y) + " " + std::to_string(vec.z) + " ";
}

void InstantiateGameObject(GameObject* gameObject, GameObject* scene, GameState* gameState)
{
    gGameObjects.push_back(gameObject);
    if(gameState->gameStarted)
    {
        gameObject->Start(scene, gameState);
    }
}

void DestroyGameObject(GameObject* gameObject)
{
    int size = gGameObjects.size();
    // for(int i = 0; i < size;)
    // {
    //     if(gGameObjects[i] == gameObject)
    //     {
    //         std::swap(gGameObjects[i], gGameObjects[--size]);
    //         gGameObjects.pop_back();
    //     }
    //     else
    //     {
    //         i++;
    //     }
    // }
    LogCustom(0, "DestroyGameObjects is called");
    LogCustom(0, std::to_string(gameObject == nullptr).c_str());
    gameObject->Destroy();
    LogCustom(0, "DestroyGameObjects finished");
}

void DestroyGameObjectDeffered(GameObject* gameObject)
{
    gGameObjectsToBeRemoved.push_back(gameObject);
}

void DestroyGameObjectsDeffered()
{
    while(gGameObjectsToBeRemoved.size() != 0)
    {
        printf(std::to_string(gGameObjectsToBeRemoved.size()).c_str());
        DestroyGameObject(gGameObjectsToBeRemoved.back()); // LLO
        LogCustom(0, "Object destroyed");
        gGameObjectsToBeRemoved.pop_back();
    }
    // LogCustom(0, "Deffered Object destruction finished");
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void LoadExternalData(Resources& resources)
{
    LogCustom(0, "Loading resources...", nullptr);
    resources.cubeTriangulated = LoadModel("../resources/Models/CubeTriangulated.obj");
    resources.smallCube = LoadModel("../resources/Models/SmallCubeTesting.obj");
    resources.smallWall = LoadModel("../resources/Models/SmallWall.obj");
    resources.placeholderTexture = LoadTexture("../resources/Textures/Placeholder256x.png"); 
    resources.pit = resources.cubeTriangulated;
    SetMaterialTexture(&resources.cubeTriangulated.materials[0], MAP_DIFFUSE, resources.placeholderTexture);
    LogCustom(0, "Loading complete", nullptr);
}

void UnloadExternalData(Resources& resources)
{
    LogCustom(0, "Unloading resources...", nullptr);
    UnloadModel(resources.cubeTriangulated);
    // UnloadModel(resources.pit);
    UnloadModel(resources.smallCube);
    UnloadModel(resources.smallWall);
    UnloadTexture(resources.placeholderTexture);
    LogCustom(0, "Unloading complete", nullptr);
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void InitializeGameSystems()
{
    SetTraceLogCallback(LOG_CALLBACK);
    gBoardController = BoardController::Get();
    gScene = new GameObject();
    gCameraController = new CameraController(CAMERA_PERSPECTIVE);
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void LoadSystemObjects(Resources& resources)
{
    // TileMap
    TileMap* tileMap = new TileMap(GRIDX, GRIDY);
    gScene->AddChild(*tileMap);
    gBoardController->Initialize(tileMap);
    
    // Tilemap Base Tile
    Tile* baseTile = new Tile();
    baseTile->LoadGameObjectModel(resources.cubeTriangulated);
    baseTile->SetPitModel(&resources.smallCube);
    baseTile->SetSolidModel(&resources.pit);
    baseTile->transform.scale.y = 0.1f;
    baseTile->baseColor = RED;
    
    // Tilemap Wall Object
    TileObject* wall = new TileObject();
    wall->LoadGameObjectModel(resources.cubeTriangulated);
    wall->baseColor = BROWN;

    tileMap->GenerateTileMap(baseTile, wall);
    gScene->AddChild(*tileMap);

    // Player 
    gPlayerController = new PlayerController();
    gPlayerController->transform.translation = {1, 0, 1};
    gPlayerController->LoadGameObjectModel(resources.cubeTriangulated);
    gPlayerController->baseColor = BLUE;
    gPlayerController->unitStats.maxHealth = 5;
    BoardController::Get()->player = gPlayerController;
    gScene->AddChild(*gPlayerController);

    // Block at enemy location
    // TileObject* block = new TileObject(*wall);
    // block->transform.translation = {5, 0 , 5};
    // gScene->AddChild(*block);

    // BasicEnemy : Testing
    {
        BasicEnemyController* enemy = new BasicEnemyController();
        enemy->name = "Basic enemy";
        enemy->transform.translation = {5, 0, 5};
        enemy->LoadGameObjectModel(resources.cubeTriangulated);
        enemy->baseColor = PINK;
        enemy->aiStats.agroRange = 5;
        enemy->aiStats.chasingRange = 10;
        gScene->AddChild(*enemy);
        {
            UnitIndicator* unitIndicator = new UnitIndicator();
            unitIndicator->transform.translation = {0, 1.0f, 0};
            unitIndicator->LoadTextureUnIn(&resources.placeholderTexture);
            enemy->AddChild(*unitIndicator);
        }
    }

    {
        BasicEnemyController* enemy = new BasicEnemyController();
        enemy->name = "Basic enemy 2";
        enemy->transform.translation = {6, 0, 6};
        enemy->LoadGameObjectModel(resources.cubeTriangulated);
        enemy->baseColor = BROWN;
        enemy->aiStats.agroRange = 5;
        enemy->aiStats.chasingRange = 10;
        enemy->unitStats.movementSpeed = 2;
        enemy->unitStats.maxHealth = 10;
        enemy->unitStats.attackSpeed = 1.5f;
        gScene->AddChild(*enemy);
        {
            UnitIndicator* unitIndicator = new UnitIndicator();
            unitIndicator->transform.translation = {0, 1.0f, 0};
            unitIndicator->LoadTextureUnIn(&resources.placeholderTexture);
            enemy->AddChild(*unitIndicator);
        }
    }

    gCameraController->transform.translation = {0.0f, 10.0f, 4.0f};
    gCameraController->target = gPlayerController;
    gScene->AddChild(*gCameraController);
}

/////////////////////////////////////////////////////////////////////////////////////////////////

int main() 
{
    Resources resources;
    gScene = new GameObject();
    gGameState = new GameState();
    gGameState->gameStarted = false;
    InitWindow(SCREENWIDTH, SCREENHEIGHT, "raylib"); 
    LoadExternalData(resources);
    
    // Initialization
    //--------------------------------------------------------------------------------------
    InitializeGameSystems();
    LoadSystemObjects(resources);

    LogCustom(0, "Starting GameObjects", nullptr);
    for(int i = 0; i < gScene->GetChildCount(); i++)
    {
        GameObject* object = gScene->GetChild(i);
        if(!object->started)
        {
            gScene->GetChild(i)->Start(gScene, gGameState);
        }
    }
    LogCustom(0, "Starting CameraController", nullptr);
    gCameraController->Start(gScene, gGameState);
    LogCustom(0, "GameObjects started, destroying failures", nullptr);
    DestroyGameObjectsDeffered();
    LogCustom(0, "Finished destroying GameObjects", nullptr);
    gGameState->gameStarted = true;

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // UpdateCamera(&camera);
        //----------------------------------------------------------------------------------

        for(int i = 0; i < gScene->GetChildCount(); i++)
        {
            GameObject* gameObject = gScene->GetChild(i);
            if(gameObject->active)
            {
                gScene->GetChild(i)->Update(gScene, gGameState); // LLO: I should create two methods for Instantiate and Destroy, declare inside GameObject, implement in main
            }
        }

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(gCameraController->GetCamera());
                gScene->Draw();
            EndMode3D();

            DrawText("This is a raylib example", 10, 40, 20, DARKGRAY);
            DrawText("Player hp: ", 10, 70, 20, DARKGRAY);
            DrawText("Camera position: ", 10, 100, 15, DARKGRAY);
            DrawText(std::to_string(gPlayerController->unitStats.currentHealth).c_str(), 150, 70, 20, RED);
            DrawText(ToString(gCameraController->transform.translation).c_str(), 150, 100, 15, DARKGRAY);
            DrawFPS(10, 10);

        EndDrawing();

        DestroyGameObjectsDeffered();

        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadExternalData(resources);
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}





// GameObject** gameObjects = new GameObject*; allocation for 1 pointer
// GameObject** gameObjects = new GameObject*[10]; with allocation for 10 pointers
// GameObject* gameObjects[10]; allocation for 10 pointers
// object[2] == *(object + 2*sizeof(object))
// object[2] = *(object+2) = *(2+object) = 2[object]
