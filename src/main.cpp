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
#include <stdio.h>
#include <time.h>
#include <vector>

const int SCREENWIDTH = 800;
const int SCREENHEIGHT = 450;
const int GRIDX = 100;
const int GRIDY = 100;

void LogCustom(int msgType, const char *text, va_list args)
{
    char timeStr[64] = { 0 };
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);

    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", tm_info);
    printf("[%s] ", timeStr);

    switch (msgType)
    {
        case LOG_INFO: printf("[INFO] : "); break;
        case LOG_ERROR: printf("[ERROR]: "); break;
        case LOG_WARNING: printf("[WARN] : "); break;
        case LOG_DEBUG: printf("[DEBUG]: "); break;
        default: break;
    }

    vprintf(text, args);
    printf("\n");
}

int main() 
{
    SetTraceLogCallback(LogCustom);
    LogCustom(0, "Logger is working correctly", nullptr);
    TileMap tempTileMap;
    BoardController* boardController = BoardController::Get();
    BoardController::Initialize(tempTileMap);
    
    // Initialization
    //--------------------------------------------------------------------------------------
    Tile tilemap[GRIDX][GRIDY];
    for(int i = 0; i < GRIDX; i++)
    {
        for (int j = 0; j < GRIDY; j++)
        {
            tilemap[i][j] = Tile({i,j}, nullptr);
        }
    }


    InitWindow(SCREENWIDTH, SCREENHEIGHT, "raylib"); // Method in core.c, why is it not static?

    Camera camera = { 0 };
    camera.position = { 10.0f, 10.0f, 8.0f };
    camera.target = { 0.0f, 0.0f, 0.0f };
    camera.up = { 0.0f, 1.0f, 0.0f };
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    Model cube = LoadModel("resources/Models/Cube.obj");

    PlayerController* player = new PlayerController();
    player->SetTempLogger(LogCustom);
    // GameObject** gameObjects = new GameObject*; allocation for 1 pointer
    // GameObject** gameObjects = new GameObject*[10]; with allocation for 10 pointers
    // GameObject* gameObjects[10]; allocation for 10 pointers
    std::vector<GameObject*> gameObjects;
    gameObjects.push_back(player);
    // object[2] == *(object + 2*sizeof(object))
    // object[2] = *(object+2) = *(2+object) = 2[object]
    gameObjects[0]->GOLoadModel(cube);
    SetCameraMode(camera, CAMERA_FREE);

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        UpdateCamera(&camera);
        //----------------------------------------------------------------------------------

        

        for(unsigned int i = 0; i < gameObjects.size(); i++)
        {
            gameObjects[i]->Update();
        }

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);
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
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}