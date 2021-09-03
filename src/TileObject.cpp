#include "TileObject.hpp"
#include "CustomLogger.hpp"

TileObject::TileObject()
{
    occupiedTile = nullptr;
    position = {0, 0};
    this->boardController = BoardController::Get();
}

void TileObject::Start()
{
    if(occupiedTile == nullptr)
    {
        if(!boardController->InitializePosition(*this))
        {
            delete this;
        }
    }
}

void TileObject::SetOccupiedTile(Tile* tile)
{
    occupiedTile = tile;
    position = occupiedTile->gridPosition;
}