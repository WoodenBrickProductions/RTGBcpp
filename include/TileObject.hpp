#ifndef TILEOBJECT_HPP
#define TILEOBJECT_HPP

#include "GameObject.hpp"
#include "Tile.hpp"

class BoardController;
class TileObject : public GameObject
{
public:
    TileObject();
    virtual ~TileObject();

protected:
    bool staticObject;
    Tile* occupiedTile;
    GridPosition position;
    BoardController* boardController;
};
#endif