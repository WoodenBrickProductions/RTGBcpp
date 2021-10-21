#include "TileObject.hpp"
#include "CustomLogger.hpp"

TileObject::TileObject()
{
    occupiedTile = nullptr;
    position = {0, 0};
    this->boardController = BoardController::Get();
}

void TileObject::Start(GameObject* scene, GameState* gameState)
{
    GameObject::Start(scene, gameState);
    
    if(occupiedTile == nullptr)
    {
        if(!boardController->InitializePosition(*this))
        {
            std::string out = "Could not initialize: " + name;
            LogCustom(3, out.c_str(), nullptr);
            OnFailedToInitialize();
        }
    }
}

void TileObject::OnFailedToInitialize()
{
    std::string out = "Destroying TileObject: " + name;
    started = false;
    LogCustom(0, out.c_str(), nullptr);
    DestroyGameObjectDeffered(this);
}

void TileObject::SetOccupiedTile(Tile* tile)
{
    occupiedTile = tile;
    position = occupiedTile->gridPosition;
}

void TileObject::Destroy()
{
    if(occupiedTile != nullptr)
    {
        occupiedTile->ClearTileObject();
    }
 
    GameObject::Destroy();
}