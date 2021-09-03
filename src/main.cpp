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

#include "raylib.h"
#include "BoardController.hpp"
#include "Tile.hpp"
#include "PlayerController.hpp"
#include "CustomLogger.hpp"
#include <stdio.h>
#include <time.h>
#include <vector>

const auto LOG_CALLBACK = LogCustom;

const int SCREENWIDTH = 800;
const int SCREENHEIGHT = 450;
const int GRIDX = 20;
const int GRIDY = 20;

int main() 
{
    SetTraceLogCallback(LOG_CALLBACK);
    LOG_CALLBACK(0, "Logger is working correctly \n", nullptr);
    TileMap* tileMap = new TileMap(GRIDX, GRIDY);
    printf("ttayas \n");
    Tile baseTile;
    TileObject wall; 
    BoardController* boardController = BoardController::Get();
    printf("ttaya \n");
    std::vector<GameObject*> gameObjects;
    
    printf("ttay \n");

    InitWindow(SCREENWIDTH, SCREENHEIGHT, "raylib"); 
    // Initialization
    //--------------------------------------------------------------------------------------


    // Camera creation
    Camera camera = { 0 };
    camera.position = { 10.0f, 10.0f, 8.0f };
    camera.target = { 0.0f, 0.0f, 0.0f };
    camera.up = { 0.0f, 1.0f, 0.0f };
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    printf("\n\n\n\n");
    Model cube = LoadModel("../resources/Models/CubeTriangulated.obj");
    Model smallCube = LoadModel("../resources/Models/SmallCubeTesting.obj");
    Model smallWall = LoadModel("../resources/Models/SmallWall.obj");
    Texture2D placeHolderTexture = LoadTexture("../resources/Textures/Placeholder256x.png"); 
    Model pit = cube;
    SetMaterialTexture(&cube.materials[0], MAP_DIFFUSE, placeHolderTexture);
    printf("\n\n\n\n");

    baseTile.GOLoadModel(cube);
    baseTile.transform.scale.y = 0.1f;
    baseTile.baseColor = RED;
    wall.baseColor = BROWN;
    baseTile.SetPitModel(&smallCube);
    baseTile.SetSolidModel(&cube);
    
    wall.GOLoadModel(cube);
    BoardController::Initialize(tileMap);
    tileMap->GenerateTileMap(&baseTile, &wall);
    printf("\nFinished generating tileMap\n");
    
    printf("TESTING\n");

    // Player creation
    PlayerController* player = new PlayerController();
    player->GOLoadModel(cube);
    player->baseColor = BLUE;
    // GameObject** gameObjects = new GameObject*; allocation for 1 pointer
    // GameObject** gameObjects = new GameObject*[10]; with allocation for 10 pointers
    // GameObject* gameObjects[10]; allocation for 10 pointers
    gameObjects.push_back(player);
    // object[2] == *(object + 2*sizeof(object))
    // object[2] = *(object+2) = *(2+object) = 2[object]

    printf("TileMap has children %d AND ",tileMap->GetChildCount());
    printf("STARTING ------- \n");
    for(auto gameObject : gameObjects)
    {
        gameObject->Start();
    }
    SetCameraMode(camera, CAMERA_FREE);

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------


    printf("Player position: %f %f", player->transform.translation.x, player->transform.translation.z);

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        UpdateCamera(&camera);
        //----------------------------------------------------------------------------------

        tileMap->Update();
        for(unsigned int i = 0; i < gameObjects.size(); i++)
        {
            gameObjects[i]->Update();
        }

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);
                // printf("Trying to draw tilemap \n");
                tileMap->Draw();
                // printf("Finished drawing tilemap \n");
                for(unsigned int i = 0; i < gameObjects.size(); i++)
                {
                    gameObjects[i]->Draw();
                }
                DrawGrid(10, 1.0f);
            EndMode3D();

            DrawText("This is a raylib example", 10, 40, 20, DARKGRAY);

            DrawFPS(10, 10);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadModel(cube);    
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}