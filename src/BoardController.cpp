#include "BoardController.hpp"
#include "TileObject.hpp"
#include "CustomLogger.hpp"

BoardController* BoardController::current = nullptr;

BoardController::BoardController()
{
    current = this;
    tileMap = nullptr;
    worldStats.worldSpacing = 1;
}

BoardController* BoardController::Get()
{
    if(current == nullptr)
    {
        current = new BoardController();
    }
    return current;
}

void BoardController::Initialize(TileMap* tileMap)
{
    printf("initialising \n");
    Get()->tileMap = tileMap;
    printf("init done \n");
}

void BoardController::Initialize(TileMap* tileMap, float worldSpacing)
{
    LogCustom(3, "Setting custom world tile spacing", nullptr);
    Get()->worldStats.worldSpacing = worldSpacing;
    tileMap->worldSpacing = worldSpacing;
    Initialize(tileMap);
}

bool BoardController::InitializePosition(TileObject& tileObject)
{
    if (!tileMap->InitializePosition(&tileObject))
    {
        printf("Couldn't place %s", tileObject.ToString().c_str());
        return false;
    }
    
    return true;
}

bool BoardController::ForceInitializePosition(TileObject& tileObject)
{
    if (!tileMap->InitializePosition(&tileObject))
    {
        Tile* tile = tileMap->GetValidTile();
        if (tile->SetTileObject(&tileObject))
        {
            tileObject.transform.translation = tile->transform.translation;
            tileObject.SetOccupiedTile(tile);
            return true;
        }
        else
        {
            printf("Couldn't place %s: didn't get valid tile", tileObject.ToString().c_str());
            return false;
        }
    }
    
    return true;
}