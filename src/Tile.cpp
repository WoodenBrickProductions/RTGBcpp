#include "Tile.hpp"
#include "GameObject.hpp"

Tile::Tile(int x, int y, GameObject* occupiedObject)
{
    this->gridPosition = {x, y};
    this->occupiedObject = occupiedObject;
}
Tile::Tile(GridPosition gridPosition, GameObject* occupiedObject)
{
    this->gridPosition = gridPosition;
    this->occupiedObject = occupiedObject;
}

Tile::~Tile() = default;