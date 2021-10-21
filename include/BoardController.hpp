#ifndef BOARDCONTROLLER_HPP
#define BOARDCONTROLLER_HPP

#include "Tilemap.hpp"
#include "WorldStats.hpp"

class TileObject;
class PlayerController;
class BoardController 
{
public:
    virtual ~BoardController() = default;
    static BoardController* Get();
    static void Initialize(TileMap* tileMap);
    static void Initialize(TileMap* tileMap, float worldTileSpacing);
    bool InitializePosition(TileObject& tileObject);
    bool ForceInitializePosition(TileObject& tileObject);
    float GetWorldTileSpacing() {return worldStats.worldSpacing;}
    Tile* GetTile(GridPosition position) {return tileMap->GetTile(position);}

    PlayerController* player;
private:
    BoardController();
    static BoardController* current;
    TileMap* tileMap;
    WorldStats worldStats;
};


#endif 