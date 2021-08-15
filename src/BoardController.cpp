#include "BoardController.hpp"

BoardController* BoardController::current = nullptr;

BoardController::BoardController()
{
    current = this;
}

BoardController* BoardController::Get()
{
    if(current == nullptr)
    {
        current = new BoardController();
    }
    return current;
}

void BoardController::Initialize(TileMap tileMap)
{
    Get()->tileMap = tileMap;
}
BoardController::~BoardController() = default;
