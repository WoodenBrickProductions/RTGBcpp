#include "Tilemap.hpp"
#include "TileObject.hpp"
#include "external/stb_perlin.h"
#include <stdio.h>
#include <stdlib.h>
#include "CustomLogger.hpp"

TileMap::TileMap()
{
    printf("wa");
    tileSpawnRate = 0.61f;
    enemySpawnRate = 0.1f;
    wallSpawning = 0.1f;
    spawnScaling = 0.05f;
    generateEnemies = false;
    generateRandomSeed = true;
    perlinSeed = 1001;
    baseTile = nullptr;
    wallObject = nullptr;
    gridx = 1;
    gridy = 1;
    worldSpacing = 1;
    tileMatrix = nullptr;

    children.push_back(new GameObject("Tiles"));
    children.push_back(new GameObject("Walls"));
    children.push_back(new GameObject("Enemies"));
    printf("Tile map child count inside constructor: %d aAAA", children.size());
}

TileMap::TileMap(int gridx, int gridy) : TileMap()
{
    this->gridx = gridx;
    this->gridy = gridy;
    this->worldSpacing = 1;
}

TileMap::TileMap(int gridx, int gridy, float worldSpacing) : TileMap(gridx, gridy)
{
    this->worldSpacing = worldSpacing;
}

TileMap::~TileMap() 
{
    printf("\nKilling TileMap");
    ClearTileMap();
}

void TileMap::InitializeTileMap(int gridx, int gridy)
{
    // tileMatrix = static_cast<Tile***>(malloc(gridx*sizeof(Tile**)));
    // for(int i = 0; i < gridy; i++)
    // {
    //     tileMatrix[i] = static_cast<Tile**>(malloc(sizeof(Tile*)*gridy));
    // }

    // tileMatrix = new Tile*[gridx][gridy];  neveikia
    tileMatrix = new Tile**[gridx];
        for(int i = 0; i < gridx; i++)
    {
        tileMatrix[i] = new Tile*[gridy]; 
    }
    
    // tileMatrix = new Tile*[gridx*gridy];
}

void TileMap::LoadTileMap()
{
    // TODO: Needs to be implemented
    InitializeTileMap(gridx, gridy);
}  

void TileMap::GenerateTileMap(Tile* baseTile, TileObject* wallObject)
{
    printf("Generation is called \n");
    if(tileMatrix != nullptr)
    {
        printf("tileMatrix is being cleared \n");
        ClearTileMap();
    }
    printf("tileMatrix was cleared \n");
    if (generateRandomSeed)
    {
        perlinSeed = GetRandomValue(0,1000);
    }

    this->baseTile = baseTile;
    this->wallObject = wallObject;
    printf("Before Initializing tile map \n");
    InitializeTileMap(gridx, gridy);
    printf("After Initializing tilemap \n");

    int success = 0;
    for (int i = 0; i < gridx; i++)
    {
        for (int j = 0; j < gridy; j++)
        {
            float spawnValue = stb_perlin_noise3_seed((i + perlinSeed) * spawnScaling, (j + perlinSeed) * spawnScaling, 0.0f, 0, 0, 0, (int) (perlinSeed))/2 + 0.5;
            if (spawnValue <= tileSpawnRate)
            {
                // Here I should only pass the model component, there is no reason to base off of the whole baseTile;
                tileMatrix[i][j] = CreateTile(i,j);
                tileMatrix[i][j]->SetStaticTile(false);
                if (spawnValue > tileSpawnRate * (1 - wallSpawning))
                {
                    TileObject* wall = new TileObject(*wallObject);
                    wall->transform.translation = Vector3{transform.translation.x + i * worldSpacing, transform.translation.y + 0,transform.translation.z + j * worldSpacing};
                    wall->SetParent(*this);
                    if(InitializePosition(wall))
                    {
                        success++;
                    }
                }
            }
            else
            {
                tileMatrix[i][j] = CreateTile(i, j);
                tileMatrix[i][j]->SetStaticTile(true);
            }
        }
    }

    printf("Number of successful wall placements: %d \n", success);
}

void TileMap::ClearTileMap()
{
    LogCustom(0, "ClearTileMap started", nullptr);
    if(tileMatrix != nullptr)
    {
        GameObject* tiles = children[0];
        GameObject* walls = children[1];
        GameObject* spawnedEnemies = children[2];

        for (int i = tiles->GetChildCount() - 1; i > -1; i--)
        {
            delete tiles->GetChild(i);
        }
        
        for (int i = walls->GetChildCount() - 1; i > -1; i--)
        {
            delete walls->GetChild(i);
        }

        LogCustom(0, "Object delete", nullptr);

        for(int i = 0; i < gridx; i++) {
            delete(tileMatrix[i]); // delete the 2nd dimension array
        }
        delete(tileMatrix); // delete a itself

        LogCustom(0, "tileMatrix delete", nullptr);

        // Needed when using malloc
        // for(int i = 0; i < gridx; i++) {
        //     free(tileMatrix[i]); // delete the 2nd dimension array
        // }
        // free(tileMatrix); // delete a itself
    }

    // for (int i = spawnedEnemies->GetChildCount() - 1; i > -1; i--)
    // {
    //     delete spawnedEnemies->GetChild(i);
    // }



    tileMatrix = nullptr;
}

// void SpawnEnemies();

float TileMap::GetWorldTileSpacing()
{
    return this->worldSpacing;
}

Tile* TileMap::GetValidTile()
{
    // int x = (int)(Random.value * gridx);
    int x = GetRandomValue(0, gridx);
    // int y = (int)(Random.value * gridy);
    int y = GetRandomValue(0, gridy);

    for (int i = x; i < gridx; i++)
    {
        for (int j = y; j < gridy; j++)
        {
            Tile* tile = tileMatrix[i][j];
            if (!tile->IsStaticTile() && tile->GetOccupiedTileObject() == nullptr)
            {
                return tileMatrix[i][j];
            }
        }
    }

    return nullptr;
}

Tile* TileMap::GetTile(GridPosition position)
{
    if (position.x >= 0 && position.x < this->gridx && position.y >= 0 && position.y < this->gridy)
    {
        return tileMatrix[position.x][position.y];
    }
    else
    {
        return nullptr;
    }
}

bool TileMap::InitializePosition(TileObject* tileObject)
{
    Vector3 worldPosition = tileObject->transform.translation;
    GridPosition position = {(int) (worldPosition.x / worldSpacing), (int) (worldPosition.z / worldSpacing)};
    Tile* tile = GetTile(position);

    if (tile == nullptr)
    {
        printf("Can't place object %s : object outside tilemap \n", tileObject->ToString().c_str());
        return false;
    }

    if (tile->IsStaticTile())
    {
        printf("Can't place object %s : tile is static at position %s \n", tileObject->ToString().c_str(),  tile->gridPosition.ToString().c_str());
        return false;
    }
    
    if (tile->GetOccupiedTileObject() != nullptr)
    {
        printf("Can't place object %s : tile already occupied at position %s \n", tileObject->ToString().c_str(),  tile->gridPosition.ToString().c_str());
        return false;
    }
    
    if (tile->SetTileObject(tileObject))
    {
        if(tileObject->tag.compare("Player") == 0)
        {
            printf("Player successfully set" + '\n');
            printf("x: %d \n", tile->gridPosition.x);
            printf("y: %d \n", tile->gridPosition.y);
            printf("Before changing translation world x,z: %f %f \n", tile->transform.translation.x, tile->transform.translation.z);
        }
        tileObject->transform.translation = tile->transform.translation;
        tileObject->SetOccupiedTile(tile);
        if(tileObject->tag.compare("Player") == 0)
        {
            printf("After changing translation world x,z: %f %f \n", tileObject->transform.translation.x, tileObject->transform.translation.z);
        }
        return true;
    }

    printf("Can't place object %s : tile already occupied at position %s", tileObject->ToString().c_str(),  tile->gridPosition.ToString().c_str());
    return false;
}

// bool TileMap::IsTileMapGenerated()
// {

// }

Tile* TileMap::CreateTile(int xPos, int zPos)
{
    Tile* tile = new Tile(*baseTile);
    tile->transform.translation = Vector3{transform.translation.x + xPos * worldSpacing, transform.translation.y + 0,transform.translation.z + zPos * worldSpacing};
    tile->SetParent(*this);
    tile->gridPosition = {xPos, zPos};
    return tile;
}