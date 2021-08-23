#ifndef BOARDCONTROLLER_HPP
#define BOARDCONTROLLER_HPP

#include "Tilemap.hpp"
class BoardController 
{
public:
    virtual ~BoardController() = default;
    static BoardController* Get();
    static void Initialize(TileMap* tileMap);
private:
    BoardController();
    static BoardController* current;
    TileMap* tileMap;
};


#endif 