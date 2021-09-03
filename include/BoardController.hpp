#ifndef BOARDCONTROLLER_HPP
#define BOARDCONTROLLER_HPP

#include "Tilemap.hpp"
#include "WorldStats.hpp"

class TileObject;
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
private:
    BoardController();
    static BoardController* current;
    TileMap* tileMap;
    WorldStats worldStats;
};


#endif 