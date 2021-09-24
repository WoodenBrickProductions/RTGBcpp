#ifndef TILE_HPP
#define TILE_HPP

#include "GameObject.hpp"

class TileObject;
struct GridPosition {
    int x;
    int y;

    std::string ToString()
    {
        std::string out = "x: ";
        out.append(std::to_string(x));
        out.append(" y: ");
        out.append(std::to_string(y));
        return out;        
    }
};

class Tile : public GameObject
{
public:
    Tile();
    // Tile(Tile& tile);
    Tile(GridPosition gridPosition, TileObject* occupiedObject);
    virtual ~Tile() = default;
    
    bool staticTile;
    GridPosition gridPosition;
    
    void Start() override;
    void Draw() override;    

    void SetSolidModel(Model* model) {solidModel = model;}
    void SetPitModel(Model* model) {pitModel = model;}
    TileObject* GetOccupiedTileObject() {return occupiedObject;}
    bool SetTileObject(TileObject* occupiedObject);
    void ClearTileObject() {occupiedObject = nullptr;}
    bool IsStaticTile() {return staticTile;}
    void SetStaticTile(bool isStatic);

private:
    TileObject* occupiedObject;
    Model* solidModel;
    Model* pitModel;
};


#endif /* TILE_H */