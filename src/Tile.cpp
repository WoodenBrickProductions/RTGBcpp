#include "Tile.hpp"

// Tile::Tile(int x, int y, GameObject* occupiedObject)
// {
//     this->gridPosition = {x, y};
//     this->occupiedObject = occupiedObject;
// }


Tile::Tile(GridPosition gridPosition, TileObject* occupiedObject)
{
    this->gridPosition = gridPosition;
    this->occupiedObject = occupiedObject;
    model = pitModel;
    name = typeid(this).name();
}

Tile::Tile() : Tile( {0} , nullptr) {}

// Tile::Tile(Tile& tile)
// {
//     this->gridPosition = tile.gridPosition;
//     this->staticTile = tile.staticTile;
//     this->occupiedObject = tile.occupiedObject;
//     this->solidModel = tile.solidModel;
//     this->pitModel = tile.pitModel;
//     this->model = model;
// }

void Tile::Draw()
{
    // printf(("Drawing: " + name).c_str());
    GameObject::Draw();
}

bool Tile::SetTileObject(TileObject* tileObject)
{
    if (!staticTile && occupiedObject == nullptr)
    {
        occupiedObject = tileObject;
        return true;
    }

    return false;
}

void Tile::SetStaticTile(bool isStatic)
{
    staticTile = isStatic;
    if (solidModel == nullptr || pitModel == nullptr)
    {
        printf("cant set static tile");
        // This should be handled properly
        return;
    }
    if (staticTile)
    {
        model = pitModel;
    }
    else
    {
        model = solidModel;
    }
}
