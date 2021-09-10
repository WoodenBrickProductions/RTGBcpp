#ifndef TILEOBJECT_HPP
#define TILEOBJECT_HPP

#include "Tile.hpp"
#include "BoardController.hpp"

class TileObject : public GameObject
{
public:
    TileObject();
    virtual ~TileObject() = default;
    void Start() override;
    bool staticObject;
    Tile* GetOccupiedTile() {return occupiedTile;}
    void SetOccupiedTile(Tile* tile);

protected:
    virtual void OnFailedToInitialize(); 
    
    Tile* occupiedTile;
    GridPosition position;
    BoardController* boardController;
};
#endif