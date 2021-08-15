#ifndef TILEMAP_HPP
#define TILEMAP_HPP

class Tile;
class TileObject;
class TileMap
{
public:
    TileMap();
    TileMap(int gridx, int gridy);
    TileMap(int gridx, int gridy, float worldSpacing);
private:
    Tile* tile;
    TileObject* wallObject;
};

#endif