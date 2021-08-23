#ifndef TILEMAP_HPP
#define TILEMAP_HPP

#include "Tile.hpp"

class TileObject;

class TileMap : public GameObject
{
public:
    TileMap();
    TileMap(int gridx, int gridy);
    TileMap(int gridx, int gridy, float worldSpacing);
    virtual ~TileMap();
    float tileSpawnRate, enemySpawnRate, wallSpawning, spawnScaling;
    bool generateEnemies, generateRandomSeed;
    float perlinSeed;

    void LoadTileMap();
    void GenerateTileMap(Tile* baseTile, TileObject* wallObject);
    void ClearTileMap();
    // void SpawnEnemies();
    float GetWorldTileSpacing();
    Tile* GetValidTile();
    Tile* GetTile(GridPosition position);
    bool InitializePosition(TileObject* tileObject);
    // bool IsTileMapGenerated();
private:
    Tile* baseTile;
    TileObject* wallObject;
    int gridx, gridy;
    float worldSpacing;
    Tile*** tileMatrix;

    Tile* CreateTile(int xPos, int zPos);
    void InitializeTileMap(int gridx, int gridy);
};

#endif