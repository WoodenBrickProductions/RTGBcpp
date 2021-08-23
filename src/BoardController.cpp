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

void BoardController::Initialize(TileMap* tileMap)
{
    printf("initialising \n");
    Get()->tileMap = tileMap;
    printf("init done \n");
}
