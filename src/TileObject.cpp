#include "TileObject.hpp"
#include "BoardController.hpp"

TileObject::TileObject()
{
    this->boardController = BoardController::Get();
}

TileObject::~TileObject() = default;