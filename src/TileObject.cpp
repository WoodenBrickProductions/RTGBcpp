#include "TileObject.hpp"
#include "BoardController.hpp"

TileObject::TileObject()
{
    this->boardController = BoardController::Get();
}

void TileObject::Start()
{

}

void TileObject::SetOccupiedTile(Tile* tile)
{
    occupiedTile = tile;
    position = occupiedTile->gridPosition;
}